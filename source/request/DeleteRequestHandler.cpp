#include "DeleteRequestHandler.hpp"

std::string DeleteRequestHandler::handle(const Request& request)
{
  (void)request;

  std::string response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: text/plain\r\n";
  response += "Content-Length: 21\r\n";
  response += "\r\n";
  response += "Resource deleted successfully";

  return response;
}