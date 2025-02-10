#include "Response.hpp"

Response::Response(int code, const std::string& message)
    : statusCode(code)
    , statusMessage(message)
{
}

void Response::setHeader(const std::string& key, const std::string& value)
{
  headers[key] = value;
}

void Response::setBody(const std::string& body)
{
  this->body = body;
}

std::string Response::buildResponse() const
{
  return "TEST";
}