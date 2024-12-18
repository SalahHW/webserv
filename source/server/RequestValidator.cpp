#include "RequestValidator.hpp"

bool RequestValidator::validate(const Request &request, Response &response) {
  if (request.getVersion() != "HTTP/1.1") {
    response.setStatusCode(505);
    response.setReasonPhrase("HTTP Version Not Supported");
    return false;
  }

  const std::string &method = request.getMethod();
  if (method != "GET" && method != "POST" && method != "DELETE") {
    response.setStatusCode(405);
    response.setReasonPhrase("Method Not Allowed");
    return false;
  }

  std::map<std::string, std::string> headers = request.getHeaders();
  if (method == "POST") {
    if (headers.find("Content-Length") == headers.end()) {
      response.setStatusCode(411);
      response.setReasonPhrase("Length Required");
      return false;
    }
    size_t contentLength = (size_t)std::atoi(headers["Content-Length"].c_str());
    if (request.getBody().size() != contentLength) {
      response.setStatusCode(400);
      response.setReasonPhrase("Bad Request: Content-Length mismatch");
      return false;
    }
  }

  if (request.getUri().size() > 2048) {
    response.setStatusCode(414);
    response.setReasonPhrase("URI Too Long");
    return false;
  }

  return true;
}
