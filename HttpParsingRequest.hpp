#ifndef HTTPPARSINGREQUEST_HPP
#define HTTPPARSINGREQUEST_HPP

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

struct HttpRequest {
  std::string method;
  std::string uri;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;
};

class HttpParsing {
private:
  std::string request;

public:
  HttpParsing(std::string requestToParse);
  ~HttpParsing();
  HttpParsing(const HttpParsing &src);
  HttpParsing &operator=(const HttpParsing &src);
  void parseRequestLine(const std::string &requestLine,
                        HttpRequest &httpRequest);
  void parseHeaders(const std::string &headerLines, HttpRequest &httpRequest);
  HttpRequest parseHttpRequest(const std::string &rawRequest);
};

#endif