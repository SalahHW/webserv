#include "HttpStatusCodeDeterminer.hpp"

#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <set>

#include "Location.hpp"
#include "ParseRequest.hpp"

HttpStatusCodeDeterminer::HttpStatusCodeDeterminer(const Server &server)
    : server(server) {}

HttpStatusCodeDeterminer::~HttpStatusCodeDeterminer() {}

void HttpStatusCodeDeterminer::determineStatusCode(
    RequestParsed &requestParsed) {
  // 1. Check for basic request validity
  if (requestParsed.method.empty() || requestParsed.uri.empty() ||
      requestParsed.version.empty()) {
    requestParsed.statusCode = BAD_REQUEST;  // 400 Bad Request
    return;
  }

  // 2. Check for supported HTTP version
  if (requestParsed.version != "HTTP/1.1") {
    requestParsed.statusCode =
        HTTP_VERSION_NOT_SUPPORTED;  // 505 HTTP Version Not Supported
    return;
  }

  // 3. Check if the method is implemented
  if (requestParsed.method != "GET" && requestParsed.method != "POST" &&
      requestParsed.method != "DELETE") {
    requestParsed.statusCode = NOT_IMPLEMENTED;  // 501 Not Implemented
    return;
  }

  // 4. Check for excessively long URI
  const size_t MAX_URI_LENGTH = 2048;  // Example limit
  if (requestParsed.uri.length() > MAX_URI_LENGTH) {
    requestParsed.statusCode = URI_TOO_LONG;  // 414 URI Too Long
    return;
  }

  // 5. Find matching location
  Location matchingLocation;
  if (!findMatchingLocation(requestParsed.uri, matchingLocation)) {
    requestParsed.statusCode = PAGE_NOT_FOUND;  // 404 Not Found
    return;
  }
  // 6. Check for redirection
  if (!matchingLocation.getRedirectionPath().empty()) {
    int returnCode = matchingLocation.getReturnCode();
    if (returnCode == MOVED_PERMANENTLY || returnCode == FOUND) {
      requestParsed.statusCode = static_cast<HttpStatusCode>(returnCode);
      return;
    }
    // Default to MOVED_PERMANENTLY if unspecified
    requestParsed.statusCode = MOVED_PERMANENTLY;
    return;
  }

  // 7. Check if method is allowed
  if (!isMethodAllowed(matchingLocation, requestParsed.method)) {
    requestParsed.statusCode = METHOD_NOT_ALLOWED;  // 405 Method Not Allowed
    return;
  }

  // 8. Check for required headers
  if ((requestParsed.method == "POST" || requestParsed.method == "PUT") &&
      requestParsed.headers.find("Content-Length") ==
          requestParsed.headers.end()) {
    requestParsed.statusCode = LENGTH_REQUIRED;  // 411 Length Required
    return;
  }

  // 9. Check for payload size
  std::map<std::string, std::string>::const_iterator it =
      requestParsed.headers.find("Content-Length");
  if (it != requestParsed.headers.end()) {
    size_t contentLength = std::strtoul(it->second.c_str(), NULL, 10);
    if (contentLength >
        static_cast<size_t>(matchingLocation.getClientMaxBodySize())) {
      requestParsed.statusCode = PAYLOAD_TOO_LARGE;  // 413 Payload Too Large
      return;
    }
  }

  // 10. Determine file path
  std::string filePath = determineFilePath(matchingLocation, requestParsed.uri);

  // 11. Check if the resource exists
  if (!fileExists(filePath)) {
    requestParsed.statusCode = PAGE_NOT_FOUND;  // 404 Not Found
    return;
  }

  // 12. Check for forbidden access
  if (!hasAccessPermissions(filePath)) {
    requestParsed.statusCode = FORBIDDEN;  // 403 Forbidden
    return;
  }

  // 13. Check for unsupported media type
  if ((requestParsed.method == "POST" || requestParsed.method == "PUT") &&
      requestParsed.headers.find("Content-Type") !=
          requestParsed.headers.end()) {
    std::string contentType = requestParsed.headers.at("Content-Type");
    if (!isSupportedMediaType(contentType)) {
      requestParsed.statusCode =
          UNSUPPORTED_MEDIA_TYPE;  // 415 Unsupported Media Type
      return;
    }
  }

  // 14. Check for Range Not Satisfiable
  if (requestParsed.headers.find("Range") != requestParsed.headers.end()) {
    struct stat statbuf;
    if (stat(filePath.c_str(), &statbuf) == 0) {
      size_t fileSize = statbuf.st_size;
      if (!isValidRange(requestParsed, fileSize)) {
        requestParsed.statusCode =
            RANGE_NOT_SATISFIABLE;  // 416 Range Not Satisfiable
        return;
      }
    } else {
      requestParsed.statusCode =
          INTERNAL_SERVER_ERROR;  // 500 Internal Server Error
      return;
    }
  }

  // 15. Additional checks can be added here
  // For example, authentication checks for UNAUTHORIZED (401)

  // If all checks pass
  requestParsed.statusCode = OK;  // 200 OK
  return;
}

bool HttpStatusCodeDeterminer::findMatchingLocation(
    const std::string &uri, Location &matchingLocation) {
  const std::map<std::string, Location> &locations = server.getLocations();
  size_t longestMatch = 0;
  bool found = false;

  for (std::map<std::string, Location>::const_iterator it = locations.begin();
       it != locations.end(); ++it) {
    const std::string &locationPath = it->first;
    if (uri.compare(0, locationPath.length(), locationPath) == 0 &&
        locationPath.length() > longestMatch) {
      longestMatch = locationPath.length();
      matchingLocation = it->second;
      found = true;
    }
  }

  return found;
}

bool HttpStatusCodeDeterminer::isMethodAllowed(const Location &location,
                                               const std::string &method) {
  if (method == "GET") return location.getGetAccepted();
  if (method == "POST") return location.getPostAccepted();
  if (method == "DELETE") return location.getDeleteAccepted();
  return false;
}

bool HttpStatusCodeDeterminer::fileExists(const std::string &filePath) {
  struct stat buffer;
  return (stat(filePath.c_str(), &buffer) == 0);
}

bool HttpStatusCodeDeterminer::hasAccessPermissions(
    const std::string &filePath) {
  return (access(filePath.c_str(), R_OK) == 0);
}

bool HttpStatusCodeDeterminer::isSupportedMediaType(
    const std::string &contentType) {
  // Define supported media types
  std::set<std::string> supportedTypes;
  supportedTypes.insert("text/plain");
  supportedTypes.insert("text/html");
  supportedTypes.insert("application/json");
  // Add other supported types as needed

  return supportedTypes.find(contentType) != supportedTypes.end();
}

bool HttpStatusCodeDeterminer::isValidRange(const RequestParsed &requestParsed,
                                            size_t fileSize) {
  // Vérifie si l'en-tête "Range" existe
  std::map<std::string, std::string>::const_iterator it =
      requestParsed.headers.find("Range");
  if (it == requestParsed.headers.end()) {
    // Pas d'en-tête Range, donc la plage est implicitement valide
    return true;
  }

  const std::string &rangeHeader = it->second;

  // Vérifie si le Range commence bien par "bytes="
  if (rangeHeader.find("bytes=") != 0) {
    return false;  // Format incorrect
  }

  std::string rangeValue = rangeHeader.substr(6);  // Enlève "bytes="
  std::string::size_type dashPos = rangeValue.find('-');
  if (dashPos == std::string::npos) {
    return false;  // Format incorrect (doit contenir un '-')
  }

  // Analyse les valeurs de la plage
  std::string startStr = rangeValue.substr(0, dashPos);
  std::string endStr = rangeValue.substr(dashPos + 1);

  size_t start = 0, end = 0;

  // Si `start` est vide, on demande la fin du fichier
  if (!startStr.empty()) {
    char *endPtr;
    start = std::strtoul(startStr.c_str(), &endPtr, 10);
    if (*endPtr != '\0' || start >= fileSize) {
      return false;  // Début de la plage hors des limites du fichier
    }
  }

  // Si `end` est vide, cela signifie jusqu'à la fin du fichier
  if (!endStr.empty()) {
    char *endPtr;
    end = std::strtoul(endStr.c_str(), &endPtr, 10);
    if (*endPtr != '\0' || end >= fileSize) {
      return false;  // Fin de la plage hors des limites du fichier
    }
    if (start > end) {
      return false;  // Début de la plage après la fin
    }
  }

  // Si tout est correct, la plage est valide
  return true;
}

std::string HttpStatusCodeDeterminer::determineFilePath(
    const Location &location, const std::string &uri) {
  std::string relativeUri = uri.substr(location.getPath().length());
  if (relativeUri.empty() || relativeUri == "/") {
    relativeUri = "/" + location.getIndexFile();
  }
  return location.getRootDirectory() + relativeUri;
}
