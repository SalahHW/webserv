#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "RequestParser.hpp"
#include "RequestValidator.hpp"
#include "Response.hpp"

class Request {
 private:
  Response response;
  Request();
  std::string method;
  std::string uri;
  std::string version;
  std::string Host;
  std::string UserAgent;
  std::string Accept;
  std::string AcceptLanguage;
  std::string AcceptEncoding;
  std::string Connection;
  std::string body;
  std::string hostName;
  bool methodGood;
  bool uriGood;
  bool versionGood;
  bool hostGood;
  bool userAgentGood;
  bool acceptGood;
  bool acceptLanguageGood;
  bool acceptEncodingGood;
  bool connectionGood;
  bool isRequestGood;

 public:
  Request(const std::string& request,
          const std::map<std::string, VirtualHost>& virtualHosts,
          const std::string& defaultVirtualHostName);
  ~Request();
  void setMethod(const std::string& method);
  void setUri(const std::string& uri);
  void setVersion(const std::string& version);
  void setHost(const std::string& hostName);
  void setUserAgent(const std::string& userAgent);
  void setAccept(const std::string& accept);
  void setAcceptLanguage(const std::string& acceptLanguage);
  void setAcceptEncoding(const std::string& acceptEncoding);
  void setConnection(const std::string& connection);
  void setBody(const std::string& body);
  void setHostName(const std::string& hostName);

  void setMethodGood(bool methodGood);
  void setUriGood(bool uriGood);
  void setVersionGood(bool versionGood);
  void setHostGood(bool hostGood);
  void setUserAgentGood(bool userAgentGood);
  void setAcceptGood(bool acceptGood);
  void setAcceptLanguageGood(bool acceptLanguageGood);
  void setAcceptEncodingGood(bool acceptEncodingGood);
  void setConnectionGood(bool connectionGood);
  void setIsRequestGood(bool isRequestGood);

  const std::string& getMethod() const;
  const std::string& getUri() const;
  const std::string& getVersion() const;
  const std::string& getHost() const;
  const std::string& getUserAgent() const;
  const std::string& getAccept() const;
  const std::string& getAcceptLanguage() const;
  const std::string& getAcceptEncoding() const;
  const std::string& getConnection() const;
  const std::string& getBody() const;
  const std::string& getHostName() const;

  bool getHostGood() const;
  bool getUserAgentGood() const;
  bool getAcceptGood() const;
  bool getAcceptLanguageGood() const;
  bool getAcceptEncodingGood() const;
  bool getConnectionGood() const;
  bool getBodyGood() const;
  bool getHostNameGood() const;
  bool getMethodGood() const;
  bool getUriGood() const;
  bool getVersionGood() const;
  bool getIsRequestGood() const;

  void displayRequest() const;
};