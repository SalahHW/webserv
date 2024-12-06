#include "ParseRequest.hpp"

#include <cstdlib>
#include <sstream>

ParseRequest::ParseRequest(const std::string& request) : request(request) {
  requestParsed.statusCode = NADA;
  parseHttpRequest();
  showHttpRequest();
}

ParseRequest::~ParseRequest() {}

RequestParsed& ParseRequest::getParsedRequest() { return this->requestParsed; }

void ParseRequest::parseHttpRequest() {
  std::string::size_type headersStartPos;
  findAndParseRequestLine(headersStartPos);

  std::string::size_type bodyStartPos;
  findAndParseHeaders(headersStartPos, bodyStartPos);
  parseRequestBody(bodyStartPos);
}

void ParseRequest::findAndParseRequestLine(
    std::string::size_type& headersStartPos) {
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
    if (!line.empty() && *line.rbegin() == '\r') {
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
    // Trouver Content-Length
    std::map<std::string, std::string>::const_iterator it =
        this->requestParsed.headers.find("Content-Length");
    if (it != this->requestParsed.headers.end()) {
      char* endptr;
      size_t contentLength = std::strtoul(it->second.c_str(), &endptr, 10);
      if (*endptr != '\0') {
        contentLength = 0;
      }

      if (contentLength > 0 &&
          this->request.size() >= bodyStartPos + contentLength) {
        this->requestParsed.body =
            this->request.substr(bodyStartPos, contentLength);
        std::cout << "Body parsed successfully. Length: " << contentLength
                  << std::endl;

        // Analyse des parties multipart si le Content-Type contient
        // "multipart/form-data"
        if (this->requestParsed.headers["Content-Type"].find(
                "multipart/form-data") != std::string::npos) {
          std::string contentType = this->requestParsed.headers["Content-Type"];
          size_t pos = contentType.find("boundary=");
          if (pos != std::string::npos) {
            std::string boundary =
                "--" + contentType.substr(pos + 9);  // Extraire le boundary

            std::cout << "Boundary extracted: " << boundary << std::endl;

            size_t posStart = this->requestParsed.body.find(boundary);
            if (posStart != std::string::npos) {
              posStart +=
                  boundary.length();  // Avancer après le premier boundary
              size_t posEnd = this->requestParsed.body.find(boundary, posStart);

              while (posEnd != std::string::npos) {
                std::string part = this->requestParsed.body.substr(
                    posStart, posEnd - posStart);

                // Affichez chaque partie (les 200 premiers caractères)
                std::cout << "Part found: " << part.substr(0, 200) << "..."
                          << std::endl;

                posStart =
                    posEnd + boundary.length();  // Passer au prochain boundary
                posEnd = this->requestParsed.body.find(boundary, posStart);
              }

              std::cout << "End of multipart parsing." << std::endl;
            } else {
              std::cerr << "Boundary not found in body!" << std::endl;
            }
          } else {
            std::cerr << "Boundary not found in Content-Type!" << std::endl;
          }
        }
      } else {
        std::cerr << "Incomplete body! Expected: "
                  << bodyStartPos + contentLength
                  << ", Got: " << this->request.size() << std::endl;
        this->requestParsed.body = "";
      }
    } else {
      std::cerr << "Content-Length header not found!" << std::endl;
      this->requestParsed.body = "";
    }
  } else {
    std::cerr << "Request method is not POST or PUT. Skipping body parsing."
              << std::endl;
    this->requestParsed.body = "";
  }
}

std::string ParseRequest::trim(const std::string& str) const {
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