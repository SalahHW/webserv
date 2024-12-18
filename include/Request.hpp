#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>

class Request {
 public:
  Request();
  ~Request();

  const std::string& getMethod() const;
  const std::string& getUri() const;
  const std::string& getVersion() const;
  const std::map<std::string, std::string>& getHeaders() const;
  const std::string& getBody() const;

  void setMethod(const std::string& methodValue);
  void setUri(const std::string& uriValue);
  void setVersion(const std::string& versionValue);
  void addHeader(const std::string& key, const std::string& value);
  void setBody(const std::string& bodyContent);

  bool isKeepAlive() const;

 private:
  std::string method;
  std::string uri;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;
};

#endif
