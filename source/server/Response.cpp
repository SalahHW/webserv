#include "Response.hpp"

Response::~Response() {}

Response::Response(Request& request,
                   const std::map<std::string, VirtualHost>& virtualHosts,
                   const std::string& defaultVirtualHostName)
    : request(request),
      statusLine(""),
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
      bytesLoad(0),
      bytesTotal(0),
      fullResponse(""),
      builder(request, *this, virtualHosts, defaultVirtualHostName) {
  (void)request;  // for test only so is not used error in compile
}

void Response::clearForChunked() {
  setStatusLine("");
  setDate("");
  setTransferEncoding("");
  setContentType("");
  setBody("");
  setLocation("");
  setAllow("");
  setRetryAfter("");
  setConnection("");
  setFullHeader("");
  setFullResponse("");
}

bool Response::isResponseFullySend() const {
  std::cout << "Response::isResponseFullySend: " << bytesSent
            << " == " << bytesTotal << std::endl;
  return bytesSent == bytesTotal;
}

void Response::setStatusLine(const std::string& statusLine) {
  this->statusLine = statusLine;
}

void Response::setDate(const std::string& date) { this->date = date; }

void Response::setContentLength(const std::string& contentLength) {
  this->contentLength = contentLength;

  std::cout << "CONTENT LENGTH = " << this->contentLength << std::endl;
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
  std::cout << "bytesTotal: " << bytesTotal << std::endl;
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

ResponseBuilder Response::getResponseBuilder() const { return builder; }

void Response::printResponseAttributes() const {
  std::cout << "Full Response: " << fullResponse << std::endl;
}
