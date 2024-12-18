#include "Response.hpp"

Response::Response() : statusCode(200), reasonPhrase("OK") {}
Response::~Response() {}

int Response::getStatusCode() const { return statusCode; }
const std::string& Response::getReasonPhrase() const { return reasonPhrase; }
const std::string& Response::getBody() const { return body; }
const std::map<std::string, std::string>& Response::getHeaders() const {
  return headers;
}

void Response::setStatusCode(int code) { statusCode = code; }
void Response::setReasonPhrase(const std::string& phrase) {
  reasonPhrase = phrase;
}
void Response::setBody(const std::string& bodyContent) { body = bodyContent; }
void Response::addHeader(const std::string& key, const std::string& value) {
  headers[key] = value;
}
