#include "RequestParser.hpp"

bool RequestParser::parse(const std::string& rawRequest, Request& request)
{
  std::stringstream requestStream(rawRequest);
  std::string line;

  if (!parseRequestLine(requestStream, request))
  {
    return false;
  }
  if (!parseHeader(requestStream, request))
  {
    return false;
  }
  parseBody(requestStream, request);
  return true;
}

bool RequestParser::parseRequestLine(std::stringstream& requestStream, Request& request)
{
  std::string line;
  if (!std::getline(requestStream, line) || line.empty())
  {
    return false;
  }

  std::istringstream requestLine(line);
  std::string method, uri, version;
  if (!(requestLine >> method >> uri >> version))
  {
    return false;
  }

  request.setMethod(method);
  request.setUri(uri);
  return true;
}

bool RequestParser::parseHeader(std::stringstream& requestStream, Request& request)
{
  std::string line;
  while (std::getline(requestStream, line) && !line.empty() && line != "\r")
  {
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos)
    {
      return false;
    }
    std::string headerName = line.substr(0, colonPos);
    std::string headerValue = line.substr(colonPos + 1);

    size_t start = headerValue.find_first_not_of(" \t\r\n");
    if (start != std::string::npos)
    {
      headerValue = headerValue.substr(start);
    }

    request.addHeader(headerName, headerValue);
  }
  return true;
}

void RequestParser::parseBody(std::stringstream& requestStream, Request& request)
{
	std::map<std::string, std::string> headers = request.getHeaders();
    if (headers.find("Content-Length") != headers.end()) {
        int contentLength;
		utils::convertToInt(headers["Content-Length"].c_str(), contentLength);
        std::string body(contentLength, '\0');
        requestStream.read(&body[0], contentLength);
        request.setBody(body);
    }
}