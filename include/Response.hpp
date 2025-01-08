#pragma once

#include <map>
#include <string>

class Response {
 private:
  int statusCode;
  std::string reasonPhrase;
  std::string body;
  std::map<std::string, std::string> headers;

 public:
  Response();
  ~Response();
  void setStatusCode(int statusCode);
  void setReasonPhrase(const std::string& reasonPhrase);
  void setBody(const std::string& body);
  void setHeaders(const std::map<std::string, std::string>& headers);
  int getStatusCode() const;
  const std::string& getReasonPhrase() const;
  const std::string& getBody() const;
  const std::map<std::string, std::string>& getHeaders() const;
  std::string toString() const;
};