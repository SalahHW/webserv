#pragma once

#include "RequestParser.hpp"
#include <iostream>
#include <map>
#include <string>

class Request
{
  public:
  Request(const std::string& rawRequest);

  void addHeader(const std::string& headerName, const std::string& headerValue);

  void setMethod(const std::string& method);
  void setUri(const std::string& uri);
  void setBody(const std::string& body);

  std::string getMethod() const;
  std::string getUri() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string getBody() const;
  void displayRequest() const;

  private:
  std::string rawRequest;
  std::string method;
  std::string uri;
  std::map<std::string, std::string> headers;
  std::string body;
};