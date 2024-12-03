#include "HeaderBuilder.hpp"

#include <iostream>
#include <sstream>

const char *HeaderBuilder::HTTP_VERSION = "HTTP/1.1";
const char *HeaderBuilder::LINE_TERMINATOR = "\r\n";
const char *HeaderBuilder::HEADER_SEPARATOR = ": ";
const int HeaderBuilder::DEFAULT_STATUS_CODE = 200;
const char *HeaderBuilder::DEFAULT_REASON_PHRASE = "OK";

HeaderBuilder::~HeaderBuilder() {}

HeaderBuilder::HeaderBuilder()
    : statusCode(DEFAULT_STATUS_CODE), reasonPhrase(DEFAULT_REASON_PHRASE) {}

void HeaderBuilder::setStatusCode(int code) { statusCode = code; }

void HeaderBuilder::setReasonPhrase(const std::string &phrase) {
  reasonPhrase = phrase;
}

void HeaderBuilder::addHeader(const std::string &key,
                              const std::string &value) {
  headers[key] = value;
}

std::string HeaderBuilder::buildStatusLine() {
  std::ostringstream statusLineStream;
  reasonPhrase = getReasonPhrase(statusCode);
  statusLineStream << HTTP_VERSION << " " << statusCode << " " << reasonPhrase
                   << LINE_TERMINATOR;
  return statusLineStream.str();
}

std::string HeaderBuilder::buildHeaders() const {
  std::ostringstream headersStream;
  std::map<std::string, std::string>::const_iterator it = headers.begin();
  for (; it != headers.end(); ++it) {
    headersStream << it->first << HEADER_SEPARATOR << it->second
                  << LINE_TERMINATOR;
  }
  return headersStream.str();
}

void HeaderBuilder::setContentType(const std::string &contentType) {
  addHeader("Content-Type", contentType);
}

void HeaderBuilder::setContentLength(size_t contentLength) {
  std::ostringstream oss;
  oss << contentLength;
  addHeader("Content-Length", oss.str());
}

std::string HeaderBuilder::getReasonPhrase(int code) {
  switch (code) {
    case 200:
      return "OK";
    case 301:
      return "Moved Permanently";
    case 302:
      return "Found";
    case 400:
      return "Bad Request";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 500:
      return "Internal Server Error";
    default:
      return "Unknown Status";
  }
}
