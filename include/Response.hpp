#pragma once

#include <map>
#include <string>

class Response
{
  public:
  Response(int code, const std::string& message);
  void setHeader(const std::string& key, const std::string& value);
  void setBody(const std::string& bodyClient);
  std::string buildResponse() const;

  private:
  int statusCode;
  std::string statusMessage;
  std::map<std::string, std::string> headers;
  std::string body;
};