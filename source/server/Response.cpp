#include "Response.hpp"

Response::~Response() {}

Response::Response()
    : bytesSent(0),
      bytesTotal(0),
      fullResponse(""),
      statusLine(""),
      date(""),
      contentLength(""),
      transferEncoding(""),
      contentType(""),
      body(""),
      location(""),
      allow(""),
      retryAfter(""),
      connection("") {
  ResponseBuilder responseBuilder;
  responseBuilder.TEST();
}

void Response::setStatusLine(const std::string& statusLine) {
  this->statusLine = statusLine;
}

void Response::setDate(const std::string& date) { this->date = date; }

void Response::setContentLength(const std::string& contentLength) {
  this->contentLength = contentLength;
}

void Response::setTransferEncoding(const std::string& transferEncoding) {
  this->transferEncoding = transferEncoding;
}

void Response::setContentType(const std::string& contentType) {
  this->contentType = contentType;
}

void Response::setBody(const std::string& body) { this->body = body; }

void Response::setLocation(const std::string& location) {
  this->location = location;
}

void Response::setAllow(const std::string& allow) { this->allow = allow; }

void Response::setRetryAfter(const std::string& retryAfter) {
  this->retryAfter = retryAfter;
}

void Response::setConnection(const std::string& connection) {
  this->connection = connection;
}

void Response::setBytesSent(int bytesSent) { this->bytesSent = bytesSent; }

void Response::setBytesTotal(int bytesTotal) { this->bytesTotal = bytesTotal; }

void Response::setFullResponse(const std::string& fullResponse) {
  this->fullResponse = fullResponse;
}

const std::string& Response::getStatusLine() const { return statusLine; }

const std::string& Response::getDate() const { return date; }

const std::string& Response::getContentLength() const { return contentLength; }

const std::string& Response::getTransferEncoding() const {
  return transferEncoding;
}

const std::string& Response::getContentType() const { return contentType; }

const std::string& Response::getBody() const { return body; }

const std::string& Response::getLocation() const { return location; }

const std::string& Response::getAllow() const { return allow; }

const std::string& Response::getRetryAfter() const { return retryAfter; }

const std::string& Response::getConnection() const { return connection; }

int Response::getBytesSent() const { return bytesSent; }

int Response::getBytesTotal() const { return bytesTotal; }

const std::string& Response::getFullResponse() const { return fullResponse; }
