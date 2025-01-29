#pragma once

#include <sys/stat.h>

#include <ctime>
#include <fstream>
#include <string>

#include "HttpException.hpp"
#include "Location.hpp"
#include "Request.hpp"

#define BUFFER 1024

class VirtualHost;
class Response;

class ResponseBuilder {
 private:
  ResponseBuilder();
  ResponseBuilder(const ResponseBuilder& src);

  Request* request;
  Response& response;
  size_t statusCode;
  const VirtualHost& virtualHost;
  Location matchingLocation;
  std::string determinedPath;
  std::ifstream file;

  void checkRequest();
  void treatAPost();
  const std::string& getReasonPhraseForCode(size_t code);
  void setStatusCode(size_t code);
  std::string determinePath();
  const VirtualHost& findMatchingVirtualHost(
      const std::map<std::string, VirtualHost>& virtualHosts,
      const std::string& defaultVirtualHostName);
  void determineStatusCode();
  bool isMethodAccepted();
  bool findMatchingLocation();
  bool doesVhostExist();
  bool doesUriExist();
  bool isRessourceAvailable();
  void appendToVector(std::vector<char>& vec, const std::string& str);

  void buildErrorContentLength();
  void buildStatusLine();
  void buildDate();
  void buildContentLength();
  void buildTransferEncoding();
  void buildContentType();
  void buildLocation();
  void buildAllow();
  void buildRetryAfter();
  void buildConnection();
  void buildBytesSent();
  void buildBytesTotal();
  void buildFullHeader();
  const std::string to_string(size_t number);
  const std::string findContentType(const std::string& fileName);
  size_t getFileSize(const std::string& fileName);
  void handleCgi();

 public:
  ResponseBuilder(Request* request, Response& response,
                  const std::map<std::string, VirtualHost>& virtualHosts,
                  const std::string& defaultVirtualHostName);
  ~ResponseBuilder();
  void buildBody();
  void buildErrorPage(size_t errorCode);
  size_t getStatusCode() const;
};