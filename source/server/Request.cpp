#include "Request.hpp"

Request::Request() : keepAlive(false), statusCode(0) {}

Request::~Request() {}

void Request::setMethod(const std::string& method) { this->method = method; }

void Request::setUri(const std::string& uri) { this->uri = uri; }

void Request::setVersion(const std::string& version) {
  this->version = version;
}

void Request::addHeader(const std::string& key, const std::string& value) {
  headers[key] = value;
}

void Request::setBody(const std::string& bodyContent) { body = bodyContent; }

void Request::setKeepAlive(bool keepAlive) { this->keepAlive = keepAlive; }

void Request::setStatusCode(int code) { this->statusCode = code; }

const std::string& Request::getMethod() const { return method; }

const std::string& Request::getUri() const { return uri; }

const std::string& Request::getVersion() const { return version; }

const std::map<std::string, std::string>& Request::getHeaders() const {
  return headers;
}

const std::string& Request::getBody() const { return body; }

bool Request::isKeepAlive() const { return keepAlive; }

int Request::getStatusCode() const { return statusCode; }
