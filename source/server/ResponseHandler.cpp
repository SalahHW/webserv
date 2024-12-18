#include "ResponseHandler.hpp"

#include <iostream>
#include <sstream>

ResponseHandler::ResponseHandler(
    const std::map<int, std::string>& errPages,
    const std::map<std::string, Location>& locations)
    : errorPages(errPages), locationMatcher(locations) {}

Response ResponseHandler::handleResponse(const std::string& method,
                                         const std::string& requestPath,
                                         int bodySize, bool keepAlive) {
  Response response;

  const Location* location = locationMatcher.findMatchingLocation(requestPath);
  if (!location) {
    response.setStatusCode(404);
    response.setReasonPhrase("Not Found");
    response.setBody(getErrorPage(404));
    response.addHeader("Content-Length",
                       intToString((int)response.getBody().size()));
    response.addHeader("Content-Type", "text/html");
    response.addHeader("Connection", "close");
    return response;
  }

  std::string filePath = buildFullPath(*location, requestPath);

  int statusCode = determineStatusCode(method, filePath, bodySize);
  response.setStatusCode(statusCode);
  response.setReasonPhrase(getReasonPhrase(statusCode));

  if (statusCode == 200) {
    std::string fileContent = fileHandler.readFile(filePath);
    if (fileContent.empty()) {
      response.setStatusCode(500);
      response.setReasonPhrase("Internal Server Error");
      response.setBody(getErrorPage(500));
    } else {
      response.setBody(fileContent);
    }
  } else {
    response.setBody(getErrorPage(statusCode));
  }

  response.addHeader("Content-Length",
                     intToString((int)response.getBody().size()));
  response.addHeader("Content-Type", "text/html");
  response.addHeader("Connection", keepAlive ? "keep-alive" : "close");

  return response;
}

std::string ResponseHandler::buildFullPath(
    const Location& location, const std::string& requestPath) const {
  std::string fullPath = location.getRootDirectory() + requestPath;
  if (!fullPath.empty() && fullPath[fullPath.size() - 1] == '/') {
    fullPath += location.getIndexFile();
  }
  return fullPath;
}

int ResponseHandler::determineStatusCode(const std::string& method,
                                         const std::string& filePath,
                                         int bodySize) const {
  if (method != "GET" && method != "POST" && method != "DELETE") return 405;
  if (!fileHandler.fileExists(filePath)) return 404;
  if (!fileHandler.isAccessible(filePath)) return 403;

  // Exemple de maxBodySize
  const int maxBodySize = 1048576;
  if (method == "POST" && bodySize > maxBodySize) return 413;

  return 200;
}

std::string ResponseHandler::getErrorPage(int statusCode) const {
  std::string errorPagePath = findErrorPage(statusCode);
  if (!errorPagePath.empty()) {
    std::string fileContent = fileHandler.readFile(errorPagePath);
    if (!fileContent.empty()) {
      return fileContent;
    }
  }
  std::ostringstream oss;
  oss << "<html><body><h1>Error " << statusCode << "</h1></body></html>";
  return oss.str();
}

std::string ResponseHandler::findErrorPage(int statusCode) const {
  std::map<int, std::string>::const_iterator it = errorPages.find(statusCode);
  if (it != errorPages.end()) {
    return it->second;
  }
  return "";
}

std::string ResponseHandler::getReasonPhrase(int statusCode) const {
  switch (statusCode) {
    case 200:
      return "OK";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 405:
      return "Method Not Allowed";
    case 413:
      return "Payload Too Large";
    case 500:
      return "Internal Server Error";
    default:
      return "Unknown Status";
  }
}

std::string ResponseHandler::intToString(int value) const {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}
