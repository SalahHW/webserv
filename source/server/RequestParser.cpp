#include "RequestParser.hpp"

void RequestParser::parse(const std::string &rawRequest, Request &request) {
  std::istringstream stream(rawRequest);
  std::string line;

  if (std::getline(stream, line)) {
    std::istringstream firstLine(line);
    std::string method, uri, version;
    firstLine >> method >> uri >> version;

    if (method.empty() || uri.empty() || version.empty()) {
      throw std::runtime_error("Invalid HTTP Request Line");
    }

    request.setMethod(method);
    request.setUri(uri);
    request.setVersion(version);
  } else {
    throw std::runtime_error("Empty HTTP Request");
  }

  while (std::getline(stream, line) && line != "\r") {
    size_t separator = line.find(":");
    if (separator != std::string::npos) {
      std::string key = line.substr(0, separator);
      std::string value = line.substr(separator + 1);
      while (!key.empty() &&
             (key[key.size() - 1] == ' ' || key[key.size() - 1] == '\r' ||
              key[key.size() - 1] == '\n'))
        key.erase(key.size() - 1);
      size_t startPos = 0;
      while (startPos < value.size() &&
             (value[startPos] == ' ' || value[startPos] == '\r' ||
              value[startPos] == '\n')) {
        startPos++;
      }
      value = value.substr(startPos);
      request.addHeader(key, value);
    } else if (!line.empty()) {
      throw std::runtime_error("Invalid Header Format");
    }
  }

  std::string body;
  while (std::getline(stream, line)) {
    body += line + "\n";
  }

  if (!body.empty() && body[body.size() - 1] == '\n') {
    body.erase(body.size() - 1);
  }

  request.setBody(body);
}
