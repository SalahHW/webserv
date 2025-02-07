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
      }
      if (value.find("multipart/form-data") != std::string::npos) {
        size_t bPos = value.find("boundary=");
        if (bPos != std::string::npos) {
          requestToFill.setBoundary(value.substr(bPos + 9));
          requestToFill.setBoundary("--" + requestToFill.getBoundary());
        }
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
      if (request.find("multipart/form-data") == std::string::npos) {
        requestToFill.setFileContent(body);
      }
      if (request.find("multipart/form-data") != std::string::npos) {
        parseMultipartBody(body);
      }
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

void RequestParser::parseMultipartBody(const std::string& bodyData) {
  std::string boundary = requestToFill.getBoundary();
  if (boundary.empty()) {
    std::cerr << "[parseMultipartBody] Boundary is empty => cannot parse.\n";
    return;
  }

  size_t startPos = bodyData.find(boundary);
  if (startPos == std::string::npos) {
    std::cerr << "[parseMultipartBody] No boundary found in body.\n";
    return;
  }
  startPos += boundary.size();

  if (startPos < bodyData.size() &&
      bodyData.compare(startPos, 2, "\r\n") == 0) {
    startPos += 2;
  }

  size_t endPos = bodyData.find(boundary, startPos);
  if (endPos == std::string::npos) {
    endPos = bodyData.find(boundary + "--", startPos);
    if (endPos == std::string::npos) {
      endPos = bodyData.size();
    }
  }

  std::string singlePart = bodyData.substr(startPos, endPos - startPos);

  parseSinglePart(singlePart);
}

void RequestParser::parseSinglePart(const std::string& part) {
  size_t dispPos = part.find("Content-Disposition:");
  if (dispPos == std::string::npos) {
    std::cerr << "[parseSinglePart] No Content-Disposition found.\n";
    return;
  }
  size_t dispLineEnd = part.find("\r\n", dispPos);
  if (dispLineEnd == std::string::npos) {
    std::cerr << "[parseSinglePart] Disposition line incomplete.\n";
    return;
  }
  std::string dispLine = part.substr(dispPos, dispLineEnd - dispPos);

  size_t filenamePos = dispLine.find("filename=\"");
  if (filenamePos != std::string::npos) {
    filenamePos += 10;
    size_t filenameEnd = dispLine.find("\"", filenamePos);
    if (filenameEnd != std::string::npos) {
      std::string filename =
          dispLine.substr(filenamePos, filenameEnd - filenamePos);
      requestToFill.setFileName(filename);
    }
  }

  size_t ctypePos = part.find("Content-Type:", dispLineEnd + 2);
  size_t ctypeLineEnd = std::string::npos;
  if (ctypePos != std::string::npos) {
    ctypeLineEnd = part.find("\r\n", ctypePos);
    if (ctypeLineEnd != std::string::npos) {
      std::string ctypeLine = part.substr(ctypePos, ctypeLineEnd - ctypePos);
      size_t colonPos = ctypeLine.find(":");
      if (colonPos != std::string::npos) {
        std::string typeVal = ctypeLine.substr(colonPos + 1);
        while (!typeVal.empty() && (typeVal[0] == ' ' || typeVal[0] == '\t')) {
          typeVal.erase(0, 1);
        }
        requestToFill.setContentType(typeVal);
      }
    }
  }

  size_t startSearch =
      (ctypeLineEnd != std::string::npos) ? ctypeLineEnd : dispLineEnd;
  size_t contentStart = part.find("\r\n\r\n", startSearch);
  if (contentStart == std::string::npos) {
    std::cerr << "[parseSinglePart] No \\r\\n\\r\\n found.\n";
    return;
  }
  contentStart += 4;

  std::string fileData = part.substr(contentStart);

  if (fileData.size() >= 2 &&
      fileData.compare(fileData.size() - 2, 2, "\r\n") == 0) {
    fileData.erase(fileData.size() - 2);
  }

  requestToFill.setFileContent(fileData);

  std::ostringstream oss;
  oss << fileData.size();
  requestToFill.setContentLength(oss.str());
}

std::string RequestParser::trim(const std::string& str) const {
  const std::string::size_type start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) return "";
  const std::string::size_type end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}