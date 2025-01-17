#pragma once

#include <string>

#include "Request.hpp"
#include "ResponseBuilder.hpp"
#include "VirtualHost.hpp"

class Response {
 private:
  std::string statusLine;
  std::string date;
  std::string contentLength;
  std::string transferEncoding;  // content-length or chunked this field can be
                                 // empty if we use contentlength
  std::string contentType;
  std::string body;
  std::string location;    // for code 3xx and 201
  std::string allow;       // for code 405
  std::string retryAfter;  // for code 429 and 503
  std::string connection;
  size_t bytesSent;
  size_t bytesLoad;
  size_t bytesTotal;
  std::string fullHeader;
  std::string fullResponse;

 public:
  Response(const Request& request,
           const std::map<std::string, VirtualHost>& virtualHosts,
           const std::string& defaultVirtualHostName);
  ~Response();

  bool isResponseFullySend() const;
  void setStatusLine(const std::string& statusLine);
  void setDate(const std::string& date);
  void setContentLength(const std::string& contentLength);
  void setTransferEncoding(const std::string& transferEncoding);
  void setContentType(const std::string& contentType);
  void setBody(const std::string& body);
  void setLocation(const std::string& location);
  void setAllow(const std::string& allow);
  void setRetryAfter(const std::string& retryAfter);
  void setConnection(const std::string& connection);
  void setBytesSent(size_t bytesSent);
  void setBytesLoad(size_t bytesLoad);
  void setBytesTotal(size_t bytesTotal);
  void setFullHeader(const std::string& fullHeader);
  void setFullResponse(const std::string& fullResponse);

  const std::string& getStatusLine() const;
  const std::string& getDate() const;
  const std::string& getContentLength() const;
  const std::string& getTransferEncoding() const;
  const std::string& getContentType() const;
  const std::string& getBody() const;
  const std::string& getLocation() const;
  const std::string& getAllow() const;
  const std::string& getRetryAfter() const;
  const std::string& getConnection() const;
  size_t getBytesSent() const;
  size_t getBytesLoad() const;
  size_t getBytesTotal() const;
  const std::string& getFullHeader() const;
  const std::string& getFullResponse() const;

  void printResponseAttributes() const;
};