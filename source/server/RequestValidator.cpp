#include "RequestValidator.hpp"

bool RequestValidator::validateRequest(const Request& request) {
  return validateMethod(request.getMethod()) && validateUri(request.getUri()) &&
         validateVersion(request.getVersion()) &&
         validateHeaders(request.getHeaders()) &&
         validateBody(request.getBody()) &&
         validateHostName(request.getHostName()) &&
         validateMethodGood(request.getMethodGood()) &&
         validateUriGood(request.getUriGood()) &&
         validateVersionGood(request.getVersionGood()) &&
         validateHeadersGood(request.getHeadersGood());
}

bool RequestValidator::validateMethod(const std::string& method) {
  return method == "GET" || method == "POST" || method == "DELETE";
}

bool RequestValidator::validateUri(const std::string& uri) {
  return uri.length() > 0;
}

bool RequestValidator::validateVersion(const std::string& version) {
  return version == "HTTP/1.1";
}

bool RequestValidator::validateHeaders(
    const std::map<std::string, std::string>& headers) {
  return headers.find("Host") != headers.end() &&
         headers.find("User-Agent") != headers.end() &&
         headers.find("Accept") != headers.end() &&
         headers.find("Accept-Language") != headers.end() &&
         headers.find("Accept-Encoding") != headers.end() &&
         headers.find("Connection") != headers.end();
}

bool RequestValidator::validateHeader(const std::string& key,
                                      const std::string& value) {
  return key == "Host" || key == "User-Agent" || key == "Accept" ||
         key == "Accept-Language" || key == "Accept-Encoding" ||
         key == "Connection";
}

bool RequestValidator::validateBody(const std::string& body) {
  return body.length() > 0;
}

bool RequestValidator::validateHostName(const std::string& hostName) {
  return hostName.length() > 0;
}

bool RequestValidator::validateMethodGood(const bool methodGood) {
  return methodGood;
}

bool RequestValidator::validateUriGood(const bool uriGood) { return uriGood; }

bool RequestValidator::validateVersionGood(const bool versionGood) {
  return versionGood;
}

bool RequestValidator::validateHeadersGood(const bool headersGood) {
  return headersGood;
}