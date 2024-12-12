#include "ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder(Request& request, const Server& server)
    : request(request), server(server), response() {}
ResponseBuilder::~ResponseBuilder() {}

Response ResponseBuilder::buildResponse() {
  if (request.getStatusCode() == 0) {
    prepareSuccessResponse();
  } else if (request.getStatusCode() == 301 || request.getStatusCode() == 302) {
    prepareRedirectionResponse();
  } else if (request.getStatusCode() >= 400 && request.getStatusCode() < 500) {
    prepareClientErrorResponse();
  } else if (request.getStatusCode() >= 500) {
    prepareServerErrorResponse();
  } else {
    prepareServerErrorResponse();
  }
  response.initialize();
  return response;
}

void ResponseBuilder::prepareSuccessResponse() {
  Location matchingLocation;
  if (!findMatchingLocation(request.getUri(), matchingLocation)) {
    request.setStatusCode(404);
    prepareClientErrorResponse();
    return;
  }

  std::string relativeUri =
      request.getUri().substr(matchingLocation.getPath().length());
  if ((relativeUri.empty() || relativeUri == "/") &&
      !matchingLocation.getIndexFile().empty()) {
    relativeUri = "/" + matchingLocation.getIndexFile();
  }

  std::string filePath = matchingLocation.getRootDirectory() + relativeUri;
  filePath = normalizePath(filePath);

  if (!fileExists(filePath)) {
    request.setStatusCode(404);
    prepareClientErrorResponse();
    return;
  }

  if (isDirectory(filePath)) {
    if (matchingLocation.getAutoIndex()) {
      std::string dirListing =
          generateDirectoryListing(filePath, request.getUri());
      response.setBody(dirListing);
      response.setContentType("text/html");
    } else {
      filePath += "/" + matchingLocation.getIndexFile();
      filePath = normalizePath(filePath);
      if (!fileExists(filePath)) {
        request.setStatusCode(404);
        prepareClientErrorResponse();
        return;
      }
      std::string fileContent = readFile(filePath);
      response.setBody(fileContent);
      response.setContentType(getContentType(filePath));
    }
  } else {
    std::string fileContent = readFile(filePath);
    response.setBody(fileContent);
    response.setContentType(getContentType(filePath));
  }

  response.setStatusCode(200);
}

void ResponseBuilder::prepareRedirectionResponse() {
  Location matchingLocation;
  if (!findMatchingLocation(request.getUri(), matchingLocation)) {
    request.setStatusCode(500);
    prepareServerErrorResponse();
    return;
  }

  response.setStatusCode(request.getStatusCode());
  response.addHeader("Location", matchingLocation.getRedirectionPath());
  response.setBody("<html><body><h1>Redirecting...</h1></body></html>");
  response.setContentType("text/html");
}

void ResponseBuilder::prepareClientErrorResponse() {
  response.setStatusCode(request.getStatusCode());

  std::map<int, std::string> errorPages = server.getErrorPages();
  std::map<int, std::string>::iterator it =
      errorPages.find(request.getStatusCode());
  if (it != errorPages.end()) {
    std::string errorContent = readFile(it->second);
    if (!errorContent.empty()) {
      response.setBody(errorContent);
      response.setContentType("text/html");
    } else {
      response.setBody(generateDefaultErrorPage(request.getStatusCode()));
      response.setContentType("text/html");
    }
  } else {
    response.setBody(generateDefaultErrorPage(request.getStatusCode()));
    response.setContentType("text/html");
  }
}

void ResponseBuilder::prepareServerErrorResponse() {
  response.setStatusCode(500);
  response.setBody(
      "<html><body><h1>500 Internal Server Error</h1></body></html>");
  response.setContentType("text/html");
}

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

bool ResponseBuilder::isDirectory(const std::string& path) const {
  struct stat s;
  if (stat(path.c_str(), &s) == 0) {
    return S_ISDIR(s.st_mode);
  }
  return false;
}

std::string ResponseBuilder::generateDirectoryListing(
    const std::string& directoryPath, const std::string& uri) const {
  std::ostringstream html;
  html << "<html><body><h1>Index of " << uri << "</h1><ul>";

  DIR* dir = opendir(directoryPath.c_str());
  if (!dir) {
    return "<html><body><h1>403 Forbidden</h1></body></html>";
  }

  struct dirent* entry;
  while ((entry = readdir(dir)) != NULL) {
    std::string name = entry->d_name;
    if (name == ".") continue;
    std::string link = uri;
    if (!link.empty() && link[link.size() - 1] != '/') {
      link += '/';
    }
    link += name;
    html << "<li><a href=\"" << link << "\">" << name << "</a></li>";
  }

  closedir(dir);
  html << "</ul></body></html>";
  return html.str();
}

std::string ResponseBuilder::readFile(const std::string& filePath) const {
  std::ifstream file(filePath.c_str(),
                     std::ios::in | std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
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
  return "application/octet-stream";
}

std::string ResponseBuilder::generateDefaultErrorPage(int statusCode) const {
  std::ostringstream oss;
  oss << "<html><body><h1>" << statusCode << " ";

  std::string reasonPhrase;
  switch (statusCode) {
    case 400:
      reasonPhrase = "Bad Request";
      break;
    case 403:
      reasonPhrase = "Forbidden";
      break;
    case 404:
      reasonPhrase = "Not Found";
      break;
    case 500:
      reasonPhrase = "Internal Server Error";
      break;
    default:
      reasonPhrase = "Error";
      break;
  }
  oss << reasonPhrase << "</h1></body></html>";
  return oss.str();
}

bool ResponseBuilder::fileExists(const std::string& filePath) const {
  struct stat buffer;
  return (stat(filePath.c_str(), &buffer) == 0);
}

std::string ResponseBuilder::normalizePath(const std::string& path) const {
  std::string normalizedPath = path;
  size_t pos;
  while ((pos = normalizedPath.find("//")) != std::string::npos) {
    normalizedPath.replace(pos, 2, "/");
  }
  return normalizedPath;
}
