#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

class Response {
 public:
  Response();
  Response(int code, const std::string& phrase);
  ~Response();

  void setStatusCode(int code);
  void setReasonPhrase(const std::string& phrase);
  void addHeader(const std::string& key, const std::string& value);
  void setContentType(const std::string& contentType);
  void setContentLength(size_t length);
  void setBody(const std::string& bodyContent);
  void setHttpVersion(const std::string& version);

  std::string toString() const;

  void initialize();

  const char* getSendData(size_t length) const;

  void updateBytesSent(size_t sent);

  bool isFullySent() const;

 private:
  std::string fullResponse;
  size_t bytesSent;
  int statusCode;
  std::string reasonPhrase;
  std::string httpVersion;
  std::map<std::string, std::string> headers;
  std::string body;

  void buildFullResponse();
};

#endif  // RESPONSE_HPP
