#include "ResponseBuilder.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

ResponseBuilder::ResponseBuilder(RequestParsed& requestParsed,
                                 const Server& server)
    : requestParsed(requestParsed), server(server) {}

ResponseBuilder::~ResponseBuilder() {}

std::string ResponseBuilder::buildResponse() {
  prepareResponse();

  std::string statusLine = headerBuilder.buildStatusLine();
  std::string headers = headerBuilder.buildHeaders();

  return statusLine + headers + "\r\n" + body;
}

void ResponseBuilder::prepareResponse() {
  HttpStatusCode statusCode = requestParsed.statusCode;

  if (statusCode == OK) {
    prepareSuccessResponse();
  } else if (statusCode == MOVED_PERMANENTLY || statusCode == FOUND) {
    prepareRedirectionResponse();
  } else if (statusCode >= 400 && statusCode < 500) {
    prepareClientErrorResponse();
  } else if (statusCode >= 500) {
    prepareServerErrorResponse();
  } else {
    prepareServerErrorResponse();
  }
}

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
    }
  } else {
    serveFile(filePath);
  }
}

void ResponseBuilder::serveFile(const std::string& filePath) {
  body = readFile(filePath);
  headerBuilder.setContentType(getContentType(filePath));
  headerBuilder.setContentLength(body.size());
  headerBuilder.setStatusCode(OK);
}

void ResponseBuilder::prepareRedirectionResponse() {
  headerBuilder.setStatusCode(requestParsed.statusCode);
  headerBuilder.addHeader("Location", matchingLocation.getRedirectionPath());
  body = "<html><body><h1>Redirecting...</h1></body></html>";
  headerBuilder.setContentLength(body.size());
  headerBuilder.setContentType("text/html");
}

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
}

void ResponseBuilder::prepareServerErrorResponse() {
  headerBuilder.setStatusCode(INTERNAL_SERVER_ERROR);
  body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
  headerBuilder.setContentLength(body.size());
  headerBuilder.setContentType("text/html");
}

bool ResponseBuilder::findMatchingLocation(const std::string& uri,
                                           Location& matchingLocation) {
  const std::map<std::string, Location>& locations = server.getLocations();
  size_t longestMatch = 0;
  bool found = false;

  std::map<std::string, Location>::const_iterator it = locations.begin();
  for (; it != locations.end(); ++it) {
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
    html << "<li><a href=\"" << uri << entry->d_name << "\">" << entry->d_name
         << "</a></li>";
  }

  closedir(dir);
  html << "</ul></body></html>";
  return html.str();
}

std::string ResponseBuilder::readFile(const std::string& filePath) const {
  std::ifstream file(filePath.c_str());
  if (!file.is_open()) {
    return "";
  }

  std::ostringstream content;
  content << file.rdbuf();
  file.close();
  return content.str();
}

std::string ResponseBuilder::getContentType(const std::string& filePath) const {
  size_t dotPos = filePath.find_last_of('.');
  if (dotPos == std::string::npos) {
    return "application/octet-stream";
  }

  std::string extension = filePath.substr(dotPos + 1);
  if (extension == "html") {
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
  }
  return "application/octet-stream";
}

std::string ResponseBuilder::generateDefaultErrorPage(int statusCode) const {
  std::ostringstream oss;
  oss << "<html><body><h1>" << statusCode << " "
      << headerBuilder.getReasonPhrase(statusCode) << "</h1></body></html>";
  return oss.str();
}

bool ResponseBuilder::fileExists(const std::string& filePath) const {
  struct stat buffer;
  return (stat(filePath.c_str(), &buffer) == 0);
}
