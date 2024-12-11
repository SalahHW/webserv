#include "ParseRequest.hpp"

ParseRequest::ParseRequest(const std::string& request)
    : request(request), parsedRequest() {
  parseRequestLine();
  parseHeaders();
  parseBody();
}

ParseRequest::~ParseRequest() {}

Request ParseRequest::getParsedRequest() const { return parsedRequest; }

void ParseRequest::parseRequestLine() {
  std::istringstream stream(request);
  std::string method, uri, version;
  stream >> method >> uri >> version;
  parsedRequest.setMethod(method);
  parsedRequest.setUri(uri);
  parsedRequest.setVersion(version);
  std::cout << "[DEBUG] Parsed request line: " << method << " " << uri << " "
            << version << std::endl;
}

void ParseRequest::parseHeaders() {
  std::string::size_type headersStart = request.find("\r\n") + 2;
  std::string::size_type headersEnd = request.find("\r\n\r\n");
  if (headersEnd == std::string::npos) return;

  std::string headers = request.substr(headersStart, headersEnd - headersStart);
  std::istringstream stream(headers);
  std::string line;
  while (std::getline(stream, line)) {
    if (!line.empty() && line[line.size() - 1] == '\r') {
      line.erase(line.size() - 1);
    }
    if (line.empty()) continue;
    size_t pos = line.find(':');
    if (pos == std::string::npos) continue;
    std::string key = trim(line.substr(0, pos));
    std::string value = trim(line.substr(pos + 1));
    parsedRequest.addHeader(key, value);
    std::cout << "[DEBUG] Parsed header: " << key << ": " << value << std::endl;
  }

  std::map<std::string, std::string>::const_iterator it =
      parsedRequest.getHeaders().find("Connection");
  if (it != parsedRequest.getHeaders().end()) {
    if (it->second == "keep-alive" ||
        (parsedRequest.getVersion() == "HTTP/1.1" && it->second != "close")) {
      parsedRequest.setKeepAlive(true);
    } else {
      parsedRequest.setKeepAlive(false);
    }
  } else {
    parsedRequest.setKeepAlive(parsedRequest.getVersion() == "HTTP/1.1");
  }
  std::cout << "[DEBUG] Keep-Alive: "
            << (parsedRequest.isKeepAlive() ? "Yes" : "No") << std::endl;
}

void ParseRequest::parseBody() {
  if (parsedRequest.getMethod() == "POST" ||
      parsedRequest.getMethod() == "PUT") {
    std::map<std::string, std::string>::const_iterator it =
        parsedRequest.getHeaders().find("Content-Length");
    if (it != parsedRequest.getHeaders().end()) {
      size_t contentLength = std::strtoul(it->second.c_str(), NULL, 10);
      std::string::size_type bodyStart = request.find("\r\n\r\n") + 4;
      if (bodyStart + contentLength <= request.size()) {
        std::string bodyContent = request.substr(bodyStart, contentLength);
        parsedRequest.setBody(bodyContent);
        std::cout << "[DEBUG] Parsed body with length: " << contentLength
                  << std::endl;
      } else {
        parsedRequest.setBody("");
        std::cout << "[DEBUG] Incomplete body." << std::endl;
      }
    } else {
      std::cout << "[DEBUG] Content-Length header not found." << std::endl;
    }
  }
}

std::string ParseRequest::trim(const std::string& str) const {
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) return "";
  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}
