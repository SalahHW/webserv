#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>

class Request {
 public:
  Request();
  ~Request();

  void setMethod(const std::string& method);
  void setUri(const std::string& uri);
  void setVersion(const std::string& version);
  void addHeader(const std::string& key, const std::string& value);
  void setBody(const std::string& bodyContent);
  void setKeepAlive(bool keepAlive);
  void setStatusCode(int code);

  const std::string& getMethod() const;
  const std::string& getUri() const;
  const std::string& getVersion() const;
  const std::map<std::string, std::string>& getHeaders() const;
  const std::string& getBody() const;
  const std::string& getHeader(const std::string& header) const;
  bool isKeepAlive() const;
  int getStatusCode() const;

 private:
  std::string method;
  std::string uri;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;
  bool keepAlive;
  int statusCode;
};

#endif  // REQUEST_HPP
