#ifndef HTTP_STATUS_CODE_DETERMINER_HPP
#define HTTP_STATUS_CODE_DETERMINER_HPP

#include <string>

#include "HttpStatusCode.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "Server.hpp"

class HttpStatusCodeDeterminer {
 public:
  HttpStatusCodeDeterminer(const Server& server);
  ~HttpStatusCodeDeterminer();
  void determineStatusCode(Request& request);

 private:
  const Server& server;

  bool findMatchingLocation(const std::string& uri, Location& matchingLocation);
  bool isMethodAllowed(const Location& location, const std::string& method);
  bool fileExists(const std::string& filePath) const;
  bool hasAccessPermissions(const std::string& filePath) const;
  bool isSupportedMediaType(const std::string& contentType) const;
  bool isValidRange(const Request& request, size_t fileSize) const;

  std::string determineFilePath(const Location& location,
                                const std::string& uri);
  std::string normalizePath(const std::string& path) const;
};

#endif  // HTTP_STATUS_CODE_DETERMINER_HPP
