#pragma once

#include <ctime>
#include <fstream>
#include <string>

#include "Request.hpp"

class Response;
class VirtualHost;

class ResponseBuilder {
 private:
  ResponseBuilder();

  const Request& request;
  Response& response;
  VirtualHost virtualHost;

  void findMatchingVirtualHost(
      const std::map<std::string, VirtualHost>& virtualHosts);
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
  const std::string& buildFullResponse();
  const std::string& to_string(const int& number);
  const std::string findContentType(const std::string& fileName);
  size_t getFileSize(const std::string& fileName);

 public:
  ResponseBuilder(const Request& request, Response& response,
                  const std::map<std::string, VirtualHost>& virtualHosts);
  ~ResponseBuilder();
};