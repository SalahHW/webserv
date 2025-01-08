#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "Request.hpp"

class RequestValidator {
 public:
  static bool validateRequest(const Request& request);
  static bool validateMethod(const std::string& method);
  static bool validateUri(const std::string& uri);
  static bool validateVersion(const std::string& version);
  static bool validateHeaders(
      const std::map<std::string, std::string>& headers);
  static bool validateHeader(const std::string& key, const std::string& value);
  static bool validateBody(const std::string& body);
  static bool validateHostName(const std::string& hostName);
  static bool validateMethodGood(const bool methodGood);
  static bool validateUriGood(const bool uriGood);
  static bool validateVersionGood(const bool versionGood);
  static bool validateHeadersGood(const bool headersGood);
};