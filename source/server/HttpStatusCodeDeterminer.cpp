#include "HttpStatusCodeDeterminer.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <set>

HttpStatusCodeDeterminer::HttpStatusCodeDeterminer(const Server& server)
    : server(server) {}

HttpStatusCodeDeterminer::~HttpStatusCodeDeterminer() {}

void HttpStatusCodeDeterminer::determineStatusCode(
    RequestParsed& requestParsed) {
  Location matchingLocation;

  if (!isRequestValid(requestParsed)) {
    requestParsed.statusCode = BAD_REQUEST;
    return;
  }

  if (!isVersionSupported(requestParsed.version)) {
    requestParsed.statusCode = HTTP_VERSION_NOT_SUPPORTED;
    return;
  }

  if (!isMethodImplemented(requestParsed.method)) {
    requestParsed.statusCode = NOT_IMPLEMENTED;
    return;
  }

  if (!isUriLengthValid(requestParsed.uri)) {
    requestParsed.statusCode = URI_TOO_LONG;
    return;
  }

  if (!findMatchingLocation(requestParsed.uri, matchingLocation)) {
    requestParsed.statusCode = PAGE_NOT_FOUND;
    return;
  }

  if (!matchingLocation.getRedirectionPath().empty()) {
    int returnCode = matchingLocation.getReturnCode();
    if (returnCode == MOVED_PERMANENTLY || returnCode == FOUND) {
      requestParsed.statusCode = static_cast<HttpStatusCode>(returnCode);
    } else {
      requestParsed.statusCode = MOVED_PERMANENTLY;
    }
    return;
  }

  if (!isMethodAllowed(matchingLocation, requestParsed.method)) {
    requestParsed.statusCode = METHOD_NOT_ALLOWED;
    return;
  }

  if (!isContentLengthValid(requestParsed)) {
    requestParsed.statusCode = LENGTH_REQUIRED;
    return;
  }

  std::string filePath = determineFilePath(matchingLocation, requestParsed.uri);

  if (!fileExists(filePath)) {
    requestParsed.statusCode = PAGE_NOT_FOUND;
    return;
  }

  if (!hasAccessPermissions(filePath)) {
    requestParsed.statusCode = FORBIDDEN;
    return;
  }

  requestParsed.statusCode = OK;
}

bool HttpStatusCodeDeterminer::isRequestValid(
    const RequestParsed& requestParsed) const {
  return !(requestParsed.method.empty() || requestParsed.uri.empty() ||
           requestParsed.version.empty());
}

bool HttpStatusCodeDeterminer::isVersionSupported(
    const std::string& version) const {
  return version == "HTTP/1.1";
}

bool HttpStatusCodeDeterminer::isMethodImplemented(
    const std::string& method) const {
  return method == "GET" || method == "POST" || method == "DELETE";
}

bool HttpStatusCodeDeterminer::isUriLengthValid(const std::string& uri) const {
  const size_t MAX_URI_LENGTH = 2048;
  return uri.length() <= MAX_URI_LENGTH;
}

bool HttpStatusCodeDeterminer::findMatchingLocation(
    const std::string& uri, Location& matchingLocation) const {
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

bool HttpStatusCodeDeterminer::isMethodAllowed(
    const Location& location, const std::string& method) const {
  if (method == "GET") return location.getGetAccepted();
  if (method == "POST") return location.getPostAccepted();
  if (method == "DELETE") return location.getDeleteAccepted();
  return false;
}

bool HttpStatusCodeDeterminer::isContentLengthValid(
    const RequestParsed& requestParsed) const {
  if ((requestParsed.method == "POST" || requestParsed.method == "PUT") &&
      requestParsed.headers.find("Content-Length") ==
          requestParsed.headers.end()) {
    return false;
  }
  return true;
}

bool HttpStatusCodeDeterminer::fileExists(const std::string& filePath) const {
  struct stat buffer;
  return (stat(filePath.c_str(), &buffer) == 0);
}

bool HttpStatusCodeDeterminer::hasAccessPermissions(
    const std::string& filePath) const {
  return (access(filePath.c_str(), R_OK) == 0);
}

std::string HttpStatusCodeDeterminer::determineFilePath(
    const Location& location, const std::string& uri) const {
  std::string relativeUri = uri.substr(location.getPath().length());
  if ((relativeUri.empty() || relativeUri == "/") &&
      !location.getIndexFile().empty()) {
    relativeUri = "/" + location.getIndexFile();
  }
  return normalizePath(location.getRootDirectory() + relativeUri);
}

std::string HttpStatusCodeDeterminer::normalizePath(
    const std::string& path) const {
  std::string normalizedPath = path;
  size_t pos;
  while ((pos = normalizedPath.find("//")) != std::string::npos) {
    normalizedPath.replace(pos, 2, "/");
  }
  // debug
  std::cout << "Normalize Path = " << normalizedPath << std::endl;
  return normalizedPath;
}
