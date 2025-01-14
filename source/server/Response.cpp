#include "Response.hpp"

Response::~Response() {}

Response::Response(const Request& request,
                   const std::map<std::string, VirtualHost>& virtualHosts)
    : statusLine(""),
      date(""),
      contentLength(""),
      transferEncoding(""),
      contentType(""),
      body(""),
      location(""),
      allow(""),
      retryAfter(""),
      connection(""),
      bytesSent(0),
      bytesTotal(0),
      fullResponse("") {
  ResponseBuilder builder(request, *this, virtualHosts);
  printResponseAttributes();
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

void Response::setBytesSent(size_t bytesSent) { this->bytesSent = bytesSent; }

void Response::setBytesLoad(size_t bytesLoad) { this->bytesLoad = bytesLoad; }

void Response::setBytesTotal(size_t bytesTotal) {
  this->bytesTotal = bytesTotal;
}

void Response::setFullHeader(const std::string& fullHeader) {
  this->fullHeader = fullHeader;
}

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

size_t Response::getBytesSent() const { return bytesSent; }

size_t Response::getBytesLoad() const { return bytesLoad; }

size_t Response::getBytesTotal() const { return bytesTotal; }

const std::string& Response::getFullHeader() const { return fullHeader; }

const std::string& Response::getFullResponse() const { return fullResponse; }

void Response::printResponseAttributes() const {
  std::cout << std::endl
            << "Printing Response Attributes:" << std::endl
            << std::endl;
  std::cout << "Status Line: " << statusLine << std::endl;
  std::cout << "Date: " << date << std::endl;
  std::cout << "Content Length: " << contentLength << std::endl;
  std::cout << "Transfer Encoding: " << transferEncoding << std::endl;
  std::cout << "Content Type: " << contentType << std::endl;
  std::cout << "Body: " << body << std::endl;
  std::cout << "Location: " << location << std::endl;
  std::cout << "Allow: " << allow << std::endl;
  std::cout << "Retry After: " << retryAfter << std::endl;
  std::cout << "Connection: " << connection << std::endl;
  std::cout << "Bytes Sent: " << bytesSent << std::endl;
  std::cout << "Bytes Total: " << bytesTotal << std::endl;
  std::cout << "Full Header: " << fullHeader << std::endl;
  std::cout << "Full Response: " << fullResponse << std::endl;
}
