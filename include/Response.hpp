
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

class Response {
 public:
  Response();
  ~Response();

  int getStatusCode() const;
  const std::string& getReasonPhrase() const;
  const std::string& getBody() const;
  const std::map<std::string, std::string>& getHeaders() const;

  void setStatusCode(int code);
  void setReasonPhrase(const std::string& phrase);
  void setBody(const std::string& bodyContent);
  void addHeader(const std::string& key, const std::string& value);

 private:
  int statusCode;
  std::string reasonPhrase;
  std::string body;
  std::map<std::string, std::string> headers;
};

#endif
