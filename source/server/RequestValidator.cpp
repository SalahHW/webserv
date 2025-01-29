#include "RequestValidator.hpp"

#include "Request.hpp"

bool RequestValidator::validateRequest(const Request& request) {
  if (request.getHostGood() && request.getMethodGood() &&
      request.getUriGood() && request.getVersionGood() &&
      request.getUserAgentGood() && request.getAcceptGood() &&
      request.getAcceptLanguageGood() && request.getAcceptEncodingGood() &&
      request.getConnectionGood()) {
    // test
    std::cout << "Request is valid" << std::endl;
    std::cout << "[DEBUG] : Request METHOD = " << request.getMethod() << std::endl;
    std::cout << "[DEBUG] : BODY SIZE = " << request.getBody().size() << std::endl;
    return true;
  } else {
    // test
    std::cout << "Request is invalid" << std::endl;
    return false;
  }
}

bool RequestValidator::validateMethod(const std::string& method) {
  std::cout << "Method: " << method << std::endl;
  return method == "GET" || method == "POST" || method == "DELETE";
}

bool RequestValidator::validateUri(const std::string& uri) {
  return uri.length() > 0;
}

bool RequestValidator::validateVersion(const std::string& version) {
  return version == "HTTP/1.1";
}

bool RequestValidator::validateHost(const std::string& host) {
  return host.length() > 0;
}

bool RequestValidator::validateUserAgent(const std::string& userAgent) {
  return userAgent.length() > 0;
}

bool RequestValidator::validateAccept(const std::string& accept) {
  return accept.length() > 0;
}

bool RequestValidator::validateAcceptLanguage(
    const std::string& acceptLanguage) {
  return acceptLanguage.length() > 0;
}

bool RequestValidator::validateAcceptEncoding(
    const std::string& acceptEncoding) {
  return acceptEncoding.length() > 0;
}

bool RequestValidator::validateConnection(const std::string& connection) {
  return connection.length() > 0;
}

bool RequestValidator::validateBody(const std::string& body) {
  return body.length() > 0;
}

bool RequestValidator::validateHostName(const std::string& hostName) {
  return hostName.length() > 0;
}