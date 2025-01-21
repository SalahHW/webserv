#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "RequestParser.hpp"
#include "RequestValidator.hpp"
#include "Response.hpp"

class Response;
class VirtualHost;

class Request {
 private:
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
  bool isParsed;
  bool isTreated;
  bool isInTreatment;
  Response* response;

 public:
  Request();
  Request(const std::string& request);
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
  void setResponse(const std::map<std::string, VirtualHost>& virtualHosts,
                   const std::string& defaultVirtualHostName);

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
  void setIsTreated(bool isTreated);
  void setIsInTreatment(bool isInProcess);
  void setIsParsed(bool isParsed);

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
  Response* getResponse() const;

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
  bool getIsTreated() const;
  bool getIsInTreatment() const;
  bool getIsParsed() const;

  void displayRequest() const;
};