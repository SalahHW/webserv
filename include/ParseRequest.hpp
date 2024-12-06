#pragma once

#include <iostream>
#include <map>
#include <string>

#include "HttpStatusCode.hpp"

struct RequestParsed {
  std::string method;
  std::string uri;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;
  HttpStatusCode statusCode;
};

class ParseRequest {
 public:
  explicit ParseRequest(const std::string& request);
  ~ParseRequest();

  RequestParsed& getParsedRequest();

 private:
  std::string request;
  RequestParsed requestParsed;

  void parseHttpRequest();
  void findAndParseRequestLine(std::string::size_type& headersStartPos);
  void findAndParseHeaders(std::string::size_type headersStartPos,
                           std::string::size_type& bodyStartPos);
  void parseRequestBody(std::string::size_type bodyStartPos);
  std::string trim(const std::string& str) const;
  void showHttpRequest() const;
};
