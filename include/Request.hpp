#pragma once

#include <iostream>
#include <map>
#include <string>

class Request
{
  public:
  Request(const std::string& rawRequest);
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