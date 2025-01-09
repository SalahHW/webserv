#include "ResponseBuilder.hpp"

// ResponseBuilder::ResponseBuilder(const Request& request) : request(request)
// {}

ResponseBuilder::ResponseBuilder() { TEST(); }

ResponseBuilder::~ResponseBuilder() {}

void ResponseBuilder::buildStatusLine() {}

void ResponseBuilder::buildDate() {}

void ResponseBuilder::buildContentLength() {}

void ResponseBuilder::buildTransferEncoding() {}

void ResponseBuilder::buildContentType() {}

void ResponseBuilder::buildBody() {}

void ResponseBuilder::buildLocation() {}

void ResponseBuilder::buildAllow() {}

void ResponseBuilder::buildRetryAfter() {}

void ResponseBuilder::buildConnection() {}

void ResponseBuilder::buildBytesSent() {}

void ResponseBuilder::buildBytesTotal() {}

void ResponseBuilder::buildFullResponse() {}

std::string ResponseBuilder::TEST() {
  std::string s =
      "HTTP/1.1 200 OK\r\nContent-Type: text/html; "
      "charset=UTF-8\r\nContent-Length: 48\r\nDate: Wed, 08 Jan 2025 12:00:00 "
      "GMT\r\nConnection: keep-alive\r\n\r\n<html><body><h1>Hello, "
      "world!</h1></body></html>";
  return s;
}
