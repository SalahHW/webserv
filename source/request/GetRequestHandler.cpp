#include "GetRequestHandler.hpp"

std::string GetRequestHandler::handle(const Request& request)
{
  (void)request;
  std::string response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: text/plain\r\n";
  response += "Content-Length: 13\r\n";
  response += "\r\n";
  response += "Hello, World!";

  return response;
}