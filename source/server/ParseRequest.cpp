#include "ParseRequest.hpp"

ParseRequest::ParseRequest(const std::string &request) : request(request) {
  parseHttpRequest();
}

ParseRequest::~ParseRequest() {}

ParseRequest::ParseRequest(const ParseRequest &src)
    : request(src.request), requestParsed(src.requestParsed) {}

ParseRequest &ParseRequest::operator=(const ParseRequest &src) {
  if (this != &src) {
    this->request = src.request;
    this->requestParsed = src.requestParsed;
  }
  return *this;
}

RequestParsed &ParseRequest::getParsedRequest() { return this->requestParsed; }

void ParseRequest::parseHttpRequest() {
  std::string::size_type headersStartPos;
  findAndParseRequestLine(headersStartPos);

  std::string::size_type bodyStartPos;
  findAndParseHeaders(headersStartPos, bodyStartPos);

  parseRequestBody(bodyStartPos);
}

void ParseRequest::findAndParseRequestLine(
    std::string::size_type &headersStartPos) {
  std::string::size_type requestLineEnd = this->request.find("\r\n");
  if (requestLineEnd == std::string::npos) {
    headersStartPos = this->request.length();
    return;
  }

  std::string requestLine = this->request.substr(0, requestLineEnd);

  std::istringstream stream(requestLine);
  stream >> this->requestParsed.method >> this->requestParsed.uri >>
      this->requestParsed.version;

  headersStartPos = requestLineEnd + 2;
}

void ParseRequest::findAndParseHeaders(std::string::size_type headersStartPos,
                                       std::string::size_type &bodyStartPos) {
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

    if (line.empty()) continue;

    std::string::size_type pos = line.find(':');
    if (pos == std::string::npos) continue;

    std::string headerName = trim(line.substr(0, pos));
    std::string headerValue = trim(line.substr(pos + 1));

    this->requestParsed.headers[headerName] = headerValue;
  }

  bodyStartPos = headersEnd + 4;
}

void ParseRequest::parseRequestBody(std::string::size_type bodyStartPos) {
  if (this->requestParsed.method == "POST" ||
      this->requestParsed.method == "PUT") {
    std::map<std::string, std::string>::const_iterator it =
        this->requestParsed.headers.find("Content-Length");
    if (it != this->requestParsed.headers.end()) {
      std::istringstream iss(it->second);
      size_t contentLength = 0;
      iss >> contentLength;

      if (contentLength > 0 &&
          this->request.size() >= bodyStartPos + contentLength) {
        this->requestParsed.body =
            this->request.substr(bodyStartPos, contentLength);
      } else {
        this->requestParsed.body = "";
      }
    } else {
      this->requestParsed.body = "";
    }
  } else {
    this->requestParsed.body = "";
  }
}

std::string ParseRequest::trim(const std::string &str) const {
  const std::string::size_type start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) return "";
  const std::string::size_type end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}

void ParseRequest::showHttpRequest() const {
  std::cout << "Method: " << this->requestParsed.method << std::endl;
  std::cout << "URI: " << this->requestParsed.uri << std::endl;
  std::cout << "Version: " << this->requestParsed.version << std::endl;
  std::cout << "Headers:" << std::endl;
  for (std::map<std::string, std::string>::const_iterator it =
           requestParsed.headers.begin();
       it != requestParsed.headers.end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
  }
  std::cout << "Body: " << this->requestParsed.body << std::endl;
}
