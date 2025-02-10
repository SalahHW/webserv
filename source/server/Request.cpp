#include "Request.hpp"

Request::Request(const std::string& rawRequest)
    : rawRequest(rawRequest)
{
}

std::string Request::getMethod() const
{
  return this->method;
}

std::string Request::getUri() const
{
  return this->uri;
}

std::map<std::string, std::string> Request::getHeaders() const
{
  return this->headers;
}

std::string Request::getBody() const
{
  return this->body;
}

void Request::displayRequest() const
{
  std::cout << rawRequest << std::endl;
}