#include "Request.hpp"

Request::Request() : method(""), uri(""), version(""), body("") {}

Request::~Request() {}

void Request::setMethod(const std::string& method) { this->method = method; }

void Request::setUri(const std::string& uri) { this->uri = uri; }

void Request::setVersion(const std::string& version) {
  this->version = version;
}

void Request::setHeaders(const std::map<std::string, std::string>& headers) {
  this->headers = headers;
}

void Request::setHeader(const std::string& name, const std::string& value) {
  if (this->headers.find(name) != this->headers.end()) {
    if (name == "Accept" || name == "Cache-Control" ||
        name == "Content-Language") {
      this->headers[name] += ", " + value;
    } else {
      this->headers[name] = value;
    }
  } else {
    this->headers[name] = value;
  }
}

void Request::setBody(const std::string& body) { this->body = body; }

void Request::setHostName(const std::string& hostName) {
  this->hostName = hostName;
}

void Request::setMethodGood(bool methodGood) { this->methodGood = methodGood; }

void Request::setUriGood(bool uriGood) { this->uriGood = uriGood; }

void Request::setVersionGood(bool versionGood) {
  this->versionGood = versionGood;
}

void Request::setHeadersGood(bool headersGood) {
  this->headersGood = headersGood;
}

const std::string& Request::getMethod() const { return this->method; }

const std::string& Request::getUri() const { return this->uri; }

const std::string& Request::getVersion() const { return this->version; }

const std::map<std::string, std::string>& Request::getHeaders() const {
  return this->headers;
}

const std::string& Request::getHeader(const std::string key) const {
  return this->headers.at(key);
}

const std::string& Request::getBody() const { return this->body; }

const std::string& Request::getHostName() const { return this->hostName; }

const bool Request::getMethodGood() const { return this->methodGood; }

const bool Request::getUriGood() const { return this->uriGood; }

const bool Request::getVersionGood() const { return this->versionGood; }

const bool Request::getHeadersGood() const { return this->headersGood; }

void Request::displayRequest() const {
  std::cout << "Method: " << this->method << std::endl;
  std::cout << "URI: " << this->uri << std::endl;
  std::cout << "Version: " << this->version << std::endl;
  std::cout << "Headers:" << std::endl;
  for (std::map<std::string, std::string>::const_iterator it = headers.begin();
       it != headers.end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
  }
  std::cout << "Body: " << this->body << std::endl;
  std::cout << "Host Name: " << this->hostName << std::endl;
}