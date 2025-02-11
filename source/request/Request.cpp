#include "Request.hpp"

Request::Request(const std::string& rawRequest)
    : rawRequest(rawRequest)
{
  RequestParser::parse(rawRequest, *this);
}

void Request::addHeader(const std::string& headerName, const std::string& headerValue)
{
  headers[headerName] = headerValue;
}

void Request::setMethod(const std::string& method)
{
  this->method = method;
}

void Request::setUri(const std::string& uri)
{
  this->uri = uri;
}

void Request::setBody(const std::string& body)
{
  this->body = body;
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
  std::cout << "Method: " << method << std::endl;
  std::cout << "URI: " << uri << std::endl;
  std::cout << "BODY: " << body << std::endl;
}