#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Request;

class RequestValidator {
 private:
  RequestValidator();

 public:
  static bool validateRequest(const Request& request);
  static bool validateMethod(const std::string& method);
  static bool validateUri(const std::string& uri);
  static bool validateVersion(const std::string& version);
  static bool validateHost(const std::string& host);
  static bool validateUserAgent(const std::string& userAgent);
  static bool validateAccept(const std::string& accept);
  static bool validateAcceptLanguage(const std::string& acceptLanguage);
  static bool validateAcceptEncoding(const std::string& acceptEncoding);
  static bool validateConnection(const std::string& connection);
  static bool validateBody(const std::string& body);
  static bool validateHostName(const std::string& hostName);
};