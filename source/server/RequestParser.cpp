#include "RequestParser.hpp"

#include "Request.hpp"

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
  std::string::size_type headersStartPos = 0;
  findAndParseRequestLine(headersStartPos);

  std::string::size_type bodyStartPos = headersStartPos;
  parseHeaders(bodyStartPos);

  parseBody(bodyStartPos);
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

void RequestParser::parseHeaders(std::string::size_type& bodyStartPos) {
  std::string::size_type pos = bodyStartPos;

  while (true) {
    std::string::size_type lineEnd = request.find("\r\n", pos);
    if (lineEnd == std::string::npos) {
      break;
    }

    if (lineEnd == pos) {
      bodyStartPos = lineEnd + 2;
      break;
    }

    std::string headerLine = request.substr(pos, lineEnd - pos);

    std::string::size_type colonPos = headerLine.find(":");
    if (colonPos != std::string::npos) {
      std::string key = trim(headerLine.substr(0, colonPos));
      std::string value = trim(headerLine.substr(colonPos + 1));

      std::string lowerKey = key;
      std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(),
                     static_cast<int (*)(int)>(std::tolower));

      if (lowerKey == "host") {
        parseHost(value);
      } else if (lowerKey == "user-agent") {
        parseUserAgent(value);
      } else if (lowerKey == "accept") {
        parseAccept(value);
      } else if (lowerKey == "accept-language") {
        parseAcceptLanguage(value);
      } else if (lowerKey == "accept-encoding") {
        parseAcceptEncoding(value);
      } else if (lowerKey == "connection") {
        parseConnection(value);
      } else if (lowerKey == "content-type") {
        requestToFill.setContentType(value);
      } else if (lowerKey == "content-length") {
        requestToFill.setContentLength(value);
      }
    }

    pos = lineEnd + 2;
  }
}

void RequestParser::parseBody(std::string::size_type bodyStartPos) {
  if (bodyStartPos < request.size()) {
    std::string body = request.substr(bodyStartPos);
    requestToFill.setBody(body);
    if (requestToFill.getMethod() == "POST") {
      requestToFill.setPostData(body);
    }
  }
}

void RequestParser::parseHost(const std::string& value) {
  requestToFill.setHost(value);
}

void RequestParser::parseUserAgent(const std::string& value) {
  requestToFill.setUserAgent(value);
}

void RequestParser::parseAccept(const std::string& value) {
  requestToFill.setAccept(value);
}

void RequestParser::parseAcceptLanguage(const std::string& value) {
  requestToFill.setAcceptLanguage(value);
}

void RequestParser::parseAcceptEncoding(const std::string& value) {
  requestToFill.setAcceptEncoding(value);
}

void RequestParser::parseConnection(const std::string& value) {
  requestToFill.setConnection(value);
}

std::string RequestParser::trim(const std::string& str) const {
  const std::string::size_type start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) return "";
  const std::string::size_type end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}