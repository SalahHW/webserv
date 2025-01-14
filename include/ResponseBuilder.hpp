#pragma once

#include <ctime>
#include <fstream>
#include <string>

#include "Location.hpp"
#include "Request.hpp"

class Response;
class VirtualHost;

class ResponseBuilder {
 private:
  ResponseBuilder();

  const Request& request;
  Response& response;
  size_t statusCode;
  const VirtualHost& virtualHost;
  Location matchingLocation;
  std::string determinePath;

  const VirtualHost& findMatchingVirtualHost(
      const std::map<std::string, VirtualHost>& virtualHosts);
  void determineStatusCode();
  bool isMethodAccepted();
  bool findMatchingLocation();
  bool doesVhostExist();
  bool doesUriExist();
  bool isRessourceAvailable();

  void buildStatusLine();
  void buildDate();
  void buildContentLength();
  void buildTransferEncoding();
  void buildContentType();
  void buildBody();
  void buildLocation();
  void buildAllow();
  void buildRetryAfter();
  void buildConnection();
  void buildBytesSent();
  void buildBytesTotal();
  void buildFullHeader();
  void buildFullResponse();
  const std::string to_string(size_t number);
  const std::string findContentType(const std::string& fileName);
  size_t getFileSize(const std::string& fileName);

 public:
  ResponseBuilder(const Request& request, Response& response,
                  const std::map<std::string, VirtualHost>& virtualHosts);
  ~ResponseBuilder();
};