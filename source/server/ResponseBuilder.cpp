// ResponseBuilder.cpp
#include "ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder(RequestParsed& requestParsed,
                                 const Server& server)
    : requestParsed(requestParsed), server(server) {
  std::cout << "[DEBUG] ResponseBuilder initialized for client." << std::endl;
}

ResponseBuilder::~ResponseBuilder() {}

// ResponseBuilder.cpp

#include <iostream>

#include "ResponseBuilder.hpp"

// Supposons que vous avez une méthode buildResponse dans votre classe
// ResponseBuilder
std::string ResponseBuilder::buildResponse() {
  prepareResponse();  // Prépare les en-têtes et le corps de la réponse

  // Construction des lignes de statut et des en-têtes
  std::string statusLine =
      headerBuilder.buildStatusLine();  // Ex: "HTTP/1.1 200 OK\r\n"
  std::string headers =
      headerBuilder
          .buildHeaders();  // Ex: "Content-Length: 5996\r\nContent-Type:
                            // text/html\r\nConnection: keep-alive\r\n"

  // Détermination de la taille du corps
  std::size_t bodySize = body.size();
  headerBuilder.setContentLength(bodySize);  // "Content-Length: 5996\r\n"

  // Reconstruction des en-têtes avec Content-Length mis à jour
  headers = headerBuilder.buildHeaders();

  // Construction complète de la réponse avec séparation correcte entre en-têtes
  // et corps
  std::string response = statusLine + headers + "\r\n" + body;

  // Calcul des tailles pour vérification
  std::size_t totalResponseSize = response.size();
  std::size_t expectedTotalSize =
      statusLine.size() + headers.size() + 2 + body.size();  // 2 pour "\r\n"

  // Vérification de la correspondance des tailles
  if (totalResponseSize != expectedTotalSize) {
    std::cerr << "[ERROR] Mismatch in response size: expected "
              << expectedTotalSize << " bytes, but got " << totalResponseSize
              << " bytes." << std::endl;
  }

  // Log pour débogage
  std::cout << "[DEBUG] Building response. Status Line: " << statusLine
            << "Headers:\n"
            << headers << "Body size: " << body.size() << " bytes."
            << std::endl;

  return response;
}

// Prepare the response based on the status code
void ResponseBuilder::prepareResponse() {
  HttpStatusCode statusCode = requestParsed.statusCode;

  if (statusCode == OK) {
    prepareSuccessResponse();
  } else if (statusCode == MOVED_PERMANENTLY || statusCode == FOUND) {
    prepareRedirectionResponse();
  } else if (statusCode >= BAD_REQUEST && statusCode < INTERNAL_SERVER_ERROR) {
    prepareClientErrorResponse();
  } else if (statusCode >= INTERNAL_SERVER_ERROR) {
    prepareServerErrorResponse();
  } else {
    // Undefined status code, treat as internal server error
    prepareServerErrorResponse();
  }
}

// Prepare a successful (200 OK) response
void ResponseBuilder::prepareSuccessResponse() {
  if (!findMatchingLocation(requestParsed.uri, matchingLocation)) {
    requestParsed.statusCode = PAGE_NOT_FOUND;
    prepareClientErrorResponse();
    return;
  }

  std::string relativeUri =
      requestParsed.uri.substr(matchingLocation.getPath().length());
  if ((relativeUri.empty() || relativeUri == "/") &&
      !matchingLocation.getIndexFile().empty()) {
    relativeUri = "/" + matchingLocation.getIndexFile();
  }

  std::string filePath = matchingLocation.getRootDirectory() + relativeUri;

  if (!fileExists(filePath)) {
    requestParsed.statusCode = PAGE_NOT_FOUND;
    prepareClientErrorResponse();
    return;
  }

  if (isDirectory(filePath)) {
    if (matchingLocation.getAutoIndex()) {
      body = generateDirectoryListing(filePath, requestParsed.uri);
      headerBuilder.setContentType("text/html");
    } else {
      filePath += "/" + matchingLocation.getIndexFile();
      serveFile(filePath);
      return;
    }
  } else {
    serveFile(filePath);
  }

  headerBuilder.setContentLength(body.size());
  std::cout << "[DEBUG] Set Content-Length to " << body.size() << " bytes."
            << std::endl;
}

// Serve a static file
void ResponseBuilder::serveFile(const std::string& filePath) {
  body = readFile(filePath);
  headerBuilder.setContentType(getContentType(filePath));
  headerBuilder.setContentLength(body.size());
  headerBuilder.setStatusCode(OK);
  std::cout << "[DEBUG] Serving file: " << filePath
            << " with size: " << body.size() << " bytes." << std::endl;
}

// Prepare a redirection response (e.g., 301, 302)
void ResponseBuilder::prepareRedirectionResponse() {
  if (!findMatchingLocation(requestParsed.uri, matchingLocation)) {
    requestParsed.statusCode = INTERNAL_SERVER_ERROR;
    prepareServerErrorResponse();
    return;
  }

  headerBuilder.setStatusCode(requestParsed.statusCode);
  headerBuilder.addHeader("Location", matchingLocation.getRedirectionPath());
  body = "<html><body><h1>Redirecting...</h1></body></html>";
  headerBuilder.setContentLength(body.size());
  headerBuilder.setContentType("text/html");
  std::cout << "[DEBUG] Prepared redirection response with status code "
            << requestParsed.statusCode << " to "
            << matchingLocation.getRedirectionPath() << std::endl;
}

// Prepare a client error response (e.g., 404 Not Found)
void ResponseBuilder::prepareClientErrorResponse() {
  headerBuilder.setStatusCode(requestParsed.statusCode);

  const std::map<int, std::string>& errorPages = server.getErrorPages();
  std::map<int, std::string>::const_iterator it =
      errorPages.find(requestParsed.statusCode);
  if (it != errorPages.end()) {
    const std::string& errorPagePath = it->second;
    body = readFile(errorPagePath);
    if (body.empty()) {
      body = generateDefaultErrorPage(requestParsed.statusCode);
      headerBuilder.setContentType("text/html");
    } else {
      headerBuilder.setContentType(getContentType(errorPagePath));
    }
  } else {
    body = generateDefaultErrorPage(requestParsed.statusCode);
    headerBuilder.setContentType("text/html");
  }

  headerBuilder.setContentLength(body.size());
  std::cout << "[DEBUG] Prepared client error response with status code "
            << requestParsed.statusCode << std::endl;
}

// Prepare a server error response (e.g., 500 Internal Server Error)
void ResponseBuilder::prepareServerErrorResponse() {
  headerBuilder.setStatusCode(INTERNAL_SERVER_ERROR);
  body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
  headerBuilder.setContentLength(body.size());
  headerBuilder.setContentType("text/html");
  std::cout
      << "[DEBUG] Prepared server error response (500 Internal Server Error)."
      << std::endl;
}

// Find the best matching location based on URI
bool ResponseBuilder::findMatchingLocation(const std::string& uri,
                                           Location& matchingLocation) {
  const std::map<std::string, Location>& locations = server.getLocations();
  size_t longestMatch = 0;
  bool found = false;

  for (std::map<std::string, Location>::const_iterator it = locations.begin();
       it != locations.end(); ++it) {
    const std::string& locationPath = it->first;
    if (uri.compare(0, locationPath.length(), locationPath) == 0 &&
        locationPath.length() > longestMatch) {
      longestMatch = locationPath.length();
      matchingLocation = it->second;
      found = true;
    }
  }

  return found;
}

// Check if a path is a directory
bool ResponseBuilder::isDirectory(const std::string& path) const {
  struct stat s;
  if (stat(path.c_str(), &s) == 0) {
    return S_ISDIR(s.st_mode);
  }
  return false;
}

// Generate a directory listing in HTML format
std::string ResponseBuilder::generateDirectoryListing(
    const std::string& directoryPath, const std::string& uri) const {
  std::ostringstream html;
  html << "<html><body><h1>Index of " << uri << "</h1><ul>";

  DIR* dir = opendir(directoryPath.c_str());
  if (!dir) {
    std::cerr << "[ERROR] Failed to open directory: " << directoryPath
              << std::endl;
    return "<html><body><h1>403 Forbidden</h1></body></html>";
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL) {
    std::string name = entry->d_name;
    if (name == ".") continue;  // Skip current directory
    std::string link = uri;
    if (!link.empty() && link[link.size() - 1] != '/') {
      link += '/';
    }
    link += name;
    html << "<li><a href=\"" << link << "\">" << name << "</a></li>";
  }

  closedir(dir);
  html << "</ul></body></html>";
  std::cout << "[DEBUG] Generated directory listing for " << directoryPath
            << std::endl;
  return html.str();
}

// Read the content of a file into a string
std::string ResponseBuilder::readFile(const std::string& filePath) const {
  std::ifstream file(filePath.c_str(),
                     std::ios::in | std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    std::cerr << "[ERROR] Failed to open file: " << filePath << std::endl;
    return "";
  }

  std::ifstream::pos_type pos = file.tellg();
  size_t fileSize = static_cast<std::size_t>(pos);
  std::string result(fileSize, '\0');
  file.seekg(0, std::ios::beg);
  file.read(&result[0], pos);
  file.close();
  return result;
}

// Determine the Content-Type based on file extension
std::string ResponseBuilder::getContentType(const std::string& filePath) const {
  size_t dotPos = filePath.find_last_of('.');
  if (dotPos == std::string::npos) {
    return "application/octet-stream";
  }

  std::string extension = filePath.substr(dotPos + 1);
  if (extension == "html" || extension == "htm") {
    return "text/html";
  } else if (extension == "css") {
    return "text/css";
  } else if (extension == "js") {
    return "application/javascript";
  } else if (extension == "png") {
    return "image/png";
  } else if (extension == "jpg" || extension == "jpeg") {
    return "image/jpeg";
  } else if (extension == "ico") {
    return "image/x-icon";
  } else if (extension == "txt") {
    return "text/plain";
  }
  // Add more as needed
  return "application/octet-stream";
}

// Generate a default error page in HTML
std::string ResponseBuilder::generateDefaultErrorPage(int statusCode) const {
  std::ostringstream oss;
  oss << "<html><body><h1>" << statusCode << " "
      << headerBuilder.getReasonPhrase(statusCode) << "</h1></body></html>";
  std::cout << "[DEBUG] Generated default error page for status code "
            << statusCode << std::endl;
  return oss.str();
}

// Check if a file exists
bool ResponseBuilder::fileExists(const std::string& filePath) const {
  struct stat buffer;
  return (stat(filePath.c_str(), &buffer) == 0);
}
