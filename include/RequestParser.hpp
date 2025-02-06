#pragma once

#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Request;

class RequestParser {
 private:
  std::string request;
  Request& requestToFill;

  void findAndParseRequestLine(std::string::size_type& headersStartPos);
  void parseHeaders(std::string::size_type& bodyStartPos);
  void parseBody(std::string::size_type bodyStartPos);
  void parseHost(const std::string& value);
  void parseUserAgent(const std::string& value);
  void parseAccept(const std::string& value);
  void parseAcceptLanguage(const std::string& value);
  void parseAcceptEncoding(const std::string& value);
  void parseConnection(const std::string& value);
  void parseMultipartBody(const std::string& bodyData);
  void parseSinglePart(const std::string& part);
  std::string trim(const std::string& str) const;

 public:
  RequestParser(const std::string& request, Request& requestToFill);
  ~RequestParser();
  RequestParser(const RequestParser& src);
  RequestParser& operator=(const RequestParser& src);

  void parseHttpRequest();
  void showHttpRequest() const;
};