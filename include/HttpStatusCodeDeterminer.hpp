#pragma once

#include "HttpStatusCode.hpp"
#include "Server.hpp"

struct RequestParsed;

class HttpStatusCodeDeterminer {
 public:
  HttpStatusCodeDeterminer(const Server& server);
  ~HttpStatusCodeDeterminer();
  void determineStatusCode(RequestParsed& RequestParsed);

 private:
  const Server& server;

  bool findMatchingLocation(const std::string& uri, Location& matchingLocation);
  bool isMethodAllowed(const Location& location, const std::string& method);
  bool fileExists(const std::string& filePath);
  bool hasAccessPermissions(const std::string& filePath);
  bool isSupportedMediaType(const std::string& contentType);
  bool isValidRange(const RequestParsed& requestParsed, size_t fileSize);
  std::string determineFilePath(const Location& location,
                                const std::string& uri);
  std::string normalizePath(const std::string& path);
};