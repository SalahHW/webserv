#include "Response.hpp"

#include <sstream>

Response::Response()
    : fullResponse(""),
      bytesSent(0),
      statusCode(200),
      reasonPhrase("OK"),
      httpVersion("HTTP/1.1"),
      headers(),
      body("") {}

Response::Response(int code, const std::string& phrase)
    : fullResponse(""),
      bytesSent(0),
      statusCode(code),
      reasonPhrase(phrase),
      httpVersion("HTTP/1.1"),
      headers(),
      body("") {}

Response::~Response() {}

void Response::setStatusCode(int code) { statusCode = code; }

void Response::setReasonPhrase(const std::string& phrase) {
  reasonPhrase = phrase;
}

void Response::addHeader(const std::string& key, const std::string& value) {
  headers[key] = value;
}

void Response::setContentType(const std::string& contentType) {
  addHeader("Content-Type", contentType);
}

void Response::setContentLength(size_t length) {
  std::ostringstream oss;
  oss << length;
  addHeader("Content-Length", oss.str());
}

void Response::setBody(const std::string& bodyContent) {
  body = bodyContent;
  setContentLength(body.size());
}

void Response::setHttpVersion(const std::string& version) {
  httpVersion = version;
}

void Response::buildFullResponse() {
  std::ostringstream responseStream;
  responseStream << httpVersion << " " << statusCode << " " << reasonPhrase
                 << "\r\n";
  for (std::map<std::string, std::string>::const_iterator it = headers.begin();
       it != headers.end(); ++it) {
    responseStream << it->first << ": " << it->second << "\r\n";
  }
  responseStream << "\r\n";
  responseStream << body;
  fullResponse = responseStream.str();
}

void Response::initialize() { buildFullResponse(); }

const char* Response::getSendData(size_t length) const {
  if (bytesSent >= fullResponse.size()) {
    length = 0;
    return NULL;
  }
  length = fullResponse.size() - bytesSent;
  return fullResponse.c_str() + bytesSent;
}

void Response::updateBytesSent(size_t sentBytes) { bytesSent += sentBytes; }

bool Response::isFullySent() const { return bytesSent >= fullResponse.size(); }