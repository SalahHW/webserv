#pragma once

#include <string>

#include "ResponseBuilder.hpp"

class Response {
 private:
  std::string statusLine;
  std::string date;
  std::string contentLength;
  std::string transferEncoding;
  std::string contentType;
  std::string body;
  std::string location;    // for code 3xx and 201
  std::string allow;       // for code 405
  std::string retryAfter;  // for code 429 and 503
  std::string connection;
  int bytesSent;
  int bytesTotal;
  std::string fullResponse;

 public:
  Response();
  ~Response();

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
  void setBytesSent(int bytesSent);
  void setBytesTotal(int bytesTotal);
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
  int getBytesSent() const;
  int getBytesTotal() const;
  const std::string& getFullResponse() const;
};