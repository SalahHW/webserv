#include "ErrorRequestHandler.hpp"

std::string ErrorRequestHandler::generateErrorResponse(int errorCode)
{
  std::string statusLine;
  std::string body;

  switch (errorCode)
  {
  case 404:
    statusLine = "HTTP/1.1 404 Not Found";
    body = "Error 404: Resource not found";
    break;
  case 405:
    statusLine = "HTTP/1.1 405 Method Not Allowed";
    body = "Error 405: Method not allowed";
    break;
  case 500:
  default:
    statusLine = "HTTP/1.1 500 Internal Server Error";
    body = "Error 500: Internal Server Error";
    break;
  }

  std::ostringstream oss;
  oss << body.size();

  std::string response = statusLine + "\r\n";
  response += "Content-Type: text/plain\r\n";
  response += "Content-Length: " + oss.str() + "\r\n";
  response += "\r\n";
  response += body;

  return response;
}