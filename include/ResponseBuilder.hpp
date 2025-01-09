#pragma once

#include "Request.hpp"

class ResponseBuilder {
 private:
  // comment for test const Request& request;
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
  void buildFullResponse();

 public:
  // ResponseBuilder(const Request& request);
  ResponseBuilder();
  ~ResponseBuilder();
  const std::string& TEST();
};