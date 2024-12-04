#pragma once

#include <string>

#include "HttpStatusCode.hpp"
#include "Location.hpp"
#include "ParseRequest.hpp"
#include "Server.hpp"

class HttpStatusCodeDeterminer {
 public:
  explicit HttpStatusCodeDeterminer(const Server& server);
  ~HttpStatusCodeDeterminer();

  void determineStatusCode(RequestParsed& requestParsed);

 private:
  const Server& server;

  bool isRequestValid(const RequestParsed& requestParsed) const;
  bool isVersionSupported(const std::string& version) const;
  bool isMethodImplemented(const std::string& method) const;
  bool isUriLengthValid(const std::string& uri) const;
  bool findMatchingLocation(const std::string& uri,
                            Location& matchingLocation) const;
  bool isMethodAllowed(const Location& location,
                       const std::string& method) const;
  bool isContentLengthValid(const RequestParsed& requestParsed) const;
  bool isMediaTypeSupported(const std::string& contentType) const;
  bool isRangeValid(const RequestParsed& requestParsed, size_t fileSize) const;
  bool fileExists(const std::string& filePath) const;
  bool hasAccessPermissions(const std::string& filePath) const;
  std::string determineFilePath(const Location& location,
                                const std::string& uri) const;
  std::string normalizePath(const std::string& path) const;
};
