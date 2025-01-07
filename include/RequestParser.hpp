#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "HttpStatusCode.hpp"
#include "Request.hpp"

class RequestParser {
 private:
  std::string request;
  Request& requestToFill;

  void findAndParseRequestLine(std::string::size_type& headersStartPos);
  void findAndParseHeaders(std::string::size_type headersStartPos,
                           std::string::size_type& bodyStartPos);
  void parseRequestBody(std::string::size_type bodyStartPos);
  std::string trim(const std::string& str) const;

 public:
  RequestParser(const std::string& request, Request& requestToFill);
  ~RequestParser();
  RequestParser(const RequestParser& src);
  RequestParser& operator=(const RequestParser& src);

  void parseHttpRequest();
  void showHttpRequest() const;
};