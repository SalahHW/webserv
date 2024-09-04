#include "HttpParsingRequest.hpp"

HttpParsing::HttpParsing(std::string requestToParse)
    : request(requestToParse) {}

HttpParsing::~HttpParsing() {}

HttpParsing::HttpParsing(const HttpParsing &src) : request(src.request) {}

HttpParsing &HttpParsing::operator=(const HttpParsing &src) {
  this->request = src.request;
  return *this;
}

void HttpParsing::parseRequestLine(const std::string &requestLine,
                                   HttpRequest &httpRequest) {}

void HttpParsing::parseHeaders(const std::string &headerLines,
                               HttpRequest &httpRequest) {}

HttpParsing HttpParsing::parseHttpRequest(const std::string &rawRequest) {}