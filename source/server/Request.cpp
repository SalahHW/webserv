#include "Request.hpp"

Request::Request() {}
Request::~Request() {}

const std::string& Request::getMethod() const { return method; }
const std::string& Request::getUri() const { return uri; }
const std::string& Request::getVersion() const { return version; }
const std::map<std::string, std::string>& Request::getHeaders() const {
  return headers;
}
const std::string& Request::getBody() const { return body; }

void Request::setMethod(const std::string& methodValue) {
  method = methodValue;
}
void Request::setUri(const std::string& uriValue) { uri = uriValue; }
void Request::setVersion(const std::string& versionValue) {
  version = versionValue;
}

void Request::addHeader(const std::string& key, const std::string& value) {
  headers[key] = value;
}

void Request::setBody(const std::string& bodyContent) { body = bodyContent; }

bool Request::isKeepAlive() const {
  std::map<std::string, std::string>::const_iterator it =
      headers.find("Connection");
  if (it != headers.end() && it->second == "keep-alive") {
    return true;
  }
  return false;
}
