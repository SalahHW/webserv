#include "RequestParser.hpp"

RequestParser::RequestParser(const std::string& request, Request& requestToFill)
    : request(request), requestToFill(requestToFill) {
  parseHttpRequest();
}

RequestParser::~RequestParser() {}

RequestParser::RequestParser(const RequestParser& src)
    : request(src.request), requestToFill(src.requestToFill) {}

RequestParser& RequestParser::operator=(const RequestParser& src) {
  if (this != &src) {
    this->request = src.request;
    this->requestToFill = src.requestToFill;
  }
  return *this;
}

void RequestParser::parseHttpRequest() {
  std::string::size_type headersStartPos;
  findAndParseRequestLine(headersStartPos);

  std::string::size_type bodyStartPos;
  findAndParseHeaders(headersStartPos, bodyStartPos);

  parseRequestBody(bodyStartPos);
}

void RequestParser::findAndParseRequestLine(
    std::string::size_type& headersStartPos) {
  std::string::size_type requestLineEnd = this->request.find("\r\n");
  if (requestLineEnd == std::string::npos) {
    headersStartPos = this->request.length();
    return;
  }

  std::string requestLine = this->request.substr(0, requestLineEnd);
  std::istringstream stream(requestLine);

  std::string method, uri, version;
  stream >> method >> uri >> version;

  this->requestToFill.setMethod(method);
  this->requestToFill.setUri(uri);
  this->requestToFill.setVersion(version);

  headersStartPos = requestLineEnd + 2;
}

void RequestParser::findAndParseHeaders(std::string::size_type headersStartPos,
                                        std::string::size_type& bodyStartPos) {
  std::string::size_type headersEnd =
      this->request.find("\r\n\r\n", headersStartPos);
  if (headersEnd == std::string::npos) {
    bodyStartPos = this->request.length();
    return;
  }
  std::string headers =
      this->request.substr(headersStartPos, headersEnd - headersStartPos);
  std::istringstream stream(headers);
  std::string line;
  while (std::getline(stream, line)) {
    if (!line.empty() && line[line.size() - 1] == '\r') {
      line.erase(line.size() - 1);
    }
    if (line.empty()) {
      continue;
    }
    std::string::size_type pos = line.find(':');
    if (pos == std::string::npos) {
      continue;
    }
    std::string headerName = trim(line.substr(0, pos));
    std::string headerValue = trim(line.substr(pos + 1));

    this->requestToFill.setHeader(headerName, headerValue);
  }
  bodyStartPos = headersEnd + 4;
}

void RequestParser::parseRequestBody(std::string::size_type bodyStartPos) {
  if (this->requestToFill.getMethod() == "POST" ||
      this->requestToFill.getMethod() == "PUT") {
    const std::map<std::string, std::string>& headers =
        this->requestToFill.getHeaders();
    std::map<std::string, std::string>::const_iterator it =
        headers.find("Content-Length");

    if (it != headers.end()) {
      std::istringstream iss(it->second);
      size_t contentLength = 0;
      iss >> contentLength;

      if (contentLength > 0 &&
          this->request.size() >= bodyStartPos + contentLength) {
        this->requestToFill.setBody(
            this->request.substr(bodyStartPos, contentLength));
      } else {
        this->requestToFill.setBody("");
      }
    } else {
      this->requestToFill.setBody("");
    }
  } else {
    this->requestToFill.setBody("");
  }
}

std::string RequestParser::trim(const std::string& str) const {
  const std::string::size_type start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) return "";
  const std::string::size_type end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}