#include "ResponseBuilder.hpp"

#include <sstream>

std::string ResponseBuilder::buildHeaders(int statusCode, size_t contentLength,
                                          const std::string& contentType,
                                          bool keepAlive) const {
  std::ostringstream headers;

  headers << "HTTP/1.1 ";
  switch (statusCode) {
    case 200:
      headers << "200 OK";
      break;
    case 403:
      headers << "403 Forbidden";
      break;
    case 404:
      headers << "404 Not Found";
      break;
    case 405:
      headers << "405 Method Not Allowed";
      break;
    case 413:
      headers << "413 Payload Too Large";
      break;
    case 500:
      headers << "500 Internal Server Error";
      break;
    default:
      headers << statusCode << " Unknown Status";
      break;
  }
  headers << "\r\n";

  headers << "Content-Length: " << contentLength << "\r\n";
  headers << "Content-Type: " << contentType << "\r\n";

  if (keepAlive) {
    headers << "Connection: keep-alive\r\n";
  } else {
    headers << "Connection: close\r\n";
  }

  headers << "\r\n";
  return headers.str();
}
