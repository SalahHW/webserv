#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Request {
 private:
  std::string method;
  std::string uri;
  std::string version;
  std::map<std::string, std::string> headers;
  std::string body;
  std::string hostName;
  bool methodGood;
  bool uriGood;
  bool versionGood;
  bool headersGood;

 public:
  Request();
  ~Request();
  void setMethod(const std::string& method);
  void setUri(const std::string& uri);
  void setVersion(const std::string& version);
  void setHeaders(const std::map<std::string, std::string>& headers);
  void setHeader(const std::string& key, const std::string& value);
  void setBody(const std::string& body);
  void setHostName(const std::string& hostName);
  void setMethodGood(bool methodGood);
  void setUriGood(bool uriGood);
  void setVersionGood(bool versionGood);
  void setHeadersGood(bool headersGood);
  const std::string& getMethod() const;
  const std::string& getUri() const;
  const std::string& getVersion() const;
  const std::map<std::string, std::string>& getHeaders() const;
  const std::string& getHeader(const std::string key) const;
  const std::string& getBody() const;
  const std::string& getHostName() const;
  const bool getMethodGood() const;
  const bool getUriGood() const;
  const bool getVersionGood() const;
  const bool getHeadersGood() const;
  void displayRequest() const;
};