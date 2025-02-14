#include "Request.hpp"

#include "Cgi.hpp"

Request::Request(const std::string& request, size_t fd)
    : fd(fd)
    , methodGood(false)
    , uriGood(false)
    , versionGood(false)
    , hostGood(false)
    , userAgentGood(false)
    , acceptGood(false)
    , acceptLanguageGood(false)
    , acceptEncodingGood(false)
    , connectionGood(false)
    , isRequestGood(false)
    , isParsed(false)
    , isTreated(false)
    , isInTreatment(false)
    , response(NULL)
{
  RequestParser requestParser(request, *this);
  methodGood = RequestValidator::validateMethod(method);
  uriGood = RequestValidator::validateUri(uri);
  versionGood = RequestValidator::validateVersion(version);
  hostGood = RequestValidator::validateHost(Host);
  userAgentGood = RequestValidator::validateUserAgent(UserAgent);
  acceptGood = RequestValidator::validateAccept(Accept);
  acceptLanguageGood = RequestValidator::validateAcceptLanguage(AcceptLanguage);
  acceptEncodingGood = RequestValidator::validateAcceptEncoding(AcceptEncoding);
  connectionGood = RequestValidator::validateConnection(Connection);
  isRequestGood = RequestValidator::validateRequest(*this);
  isParsed = true;
  // displayRequest();
}

Request::Request() { }

Request::~Request()
{
  if (response)
  {
    delete response;
  }
}

void Request::setMethod(const std::string& method) { this->method = method; }

void Request::setUri(const std::string& uri) { this->uri = uri; }

void Request::setVersion(const std::string& version)
{
  this->version = version;
}

void Request::setHost(const std::string& hostName)
{
  this->Host = hostName;
  setHostName(hostName.substr(0, hostName.find(':')));
}

void Request::setUserAgent(const std::string& userAgent)
{
  this->UserAgent = userAgent;
}

void Request::setAccept(const std::string& accept) { this->Accept = accept; }

void Request::setAcceptLanguage(const std::string& acceptLanguage)
{
  this->AcceptLanguage = acceptLanguage;
}

void Request::setAcceptEncoding(const std::string& acceptEncoding)
{
  this->AcceptEncoding = acceptEncoding;
}

void Request::setConnection(const std::string& connection)
{
  this->Connection = connection;
}

void Request::setBody(const std::string& body) { this->body = body; }

void Request::setHostName(const std::string& hostName)
{
  this->hostName = hostName;
}

void Request::setResponse(
    const std::map<std::string, VirtualHost>& virtualHosts,
    const std::string& defaultVirtualHostName)
{
  this->response = new Response(this, virtualHosts, defaultVirtualHostName);
  if (getMethod() != "POST")
  {
    isInTreatment = true;
  }
}

void Request::setMethodGood(bool methodGood) { this->methodGood = methodGood; }

void Request::setUriGood(bool uriGood) { this->uriGood = uriGood; }

void Request::setVersionGood(bool versionGood)
{
  this->versionGood = versionGood;
}

void Request::setHostGood(bool hostGood) { this->hostGood = hostGood; }

void Request::setUserAgentGood(bool userAgentGood)
{
  this->userAgentGood = userAgentGood;
}

void Request::setAcceptGood(bool acceptGood) { this->acceptGood = acceptGood; }

void Request::setAcceptLanguageGood(bool acceptLanguageGood)
{
  this->acceptLanguageGood = acceptLanguageGood;
}

void Request::setAcceptEncodingGood(bool acceptEncodingGood)
{
  this->acceptEncodingGood = acceptEncodingGood;
}

void Request::setConnectionGood(bool connectionGood)
{
  this->connectionGood = connectionGood;
}

void Request::setIsRequestGood(bool isRequestGood)
{
  this->isRequestGood = isRequestGood;
}

void Request::setIsTreated(bool isTreated) { this->isTreated = isTreated; }

void Request::setIsInTreatment(bool isInProcess)
{
  this->isInTreatment = isInProcess;
}

void Request::setIsACgi(bool isACgi) { this->isACgi = isACgi; }

void Request::setIsParsed(bool isParsed) { this->isParsed = isParsed; }

void Request::setContentType(const std::string& type) { contentType = type; }

void Request::setContentLength(const std::string& length)
{
  contentLength = length;
}

void Request::setBoundary(const std::string& boundary)
{
  this->boundary = boundary;
}

void Request::setFileName(const std::string& fileName)
{
  this->fileName = fileName;
}

void Request::setFileContent(const std::string& fileContent)
{
  this->fileContent = fileContent;
}

const std::string& Request::getContentType() const { return contentType; }

const std::string& Request::getContentLength() const { return contentLength; }

const std::string& Request::getMethod() const { return this->method; }

const std::string& Request::getUri() const { return this->uri; }

const std::string& Request::getVersion() const { return this->version; }

const std::string& Request::getHost() const { return this->Host; }

const std::string& Request::getUserAgent() const { return this->UserAgent; }

const std::string& Request::getAccept() const { return this->Accept; }

const std::string& Request::getAcceptLanguage() const
{
  return this->AcceptLanguage;
}

const std::string& Request::getAcceptEncoding() const
{
  return this->AcceptEncoding;
}

const std::string& Request::getConnection() const { return this->Connection; }

const std::string& Request::getBody() const { return this->body; }

Response* Request::getResponse() const { return response; }

const std::string& Request::getHostName() const { return this->hostName; }

size_t Request::getFd() const { return this->fd; }

const std::string& Request::getBoundary() const { return this->boundary; }

const std::string& Request::getFileName() const { return this->fileName; }

const std::string& Request::getFileContent() const { return this->fileContent; }

bool Request::getMethodGood() const { return this->methodGood; }

bool Request::getUriGood() const { return this->uriGood; }

bool Request::getVersionGood() const { return this->versionGood; }

bool Request::getHostGood() const { return this->hostGood; }

bool Request::getUserAgentGood() const { return this->userAgentGood; }

bool Request::getAcceptGood() const { return this->acceptGood; }

bool Request::getAcceptLanguageGood() const { return this->acceptLanguageGood; }

bool Request::getAcceptEncodingGood() const { return this->acceptEncodingGood; }

bool Request::getConnectionGood() const { return this->connectionGood; }

bool Request::getIsRequestGood() const { return isRequestGood; }

bool Request::getIsTreated() const { return isTreated; }

bool Request::getIsInTreatment() const { return isInTreatment; }

bool Request::getIsParsed() const { return isParsed; }

bool Request::getIsACgi() const { return isACgi; }

void Request::displayRequest() const
{
  std::cout << "Method: " << this->method << std::endl;
  std::cout << "URI: " << this->uri << std::endl;
  std::cout << "Version: " << this->version << std::endl;
  std::cout << "Host: " << this->Host << std::endl;
  std::cout << "User Agent: " << this->UserAgent << std::endl;
  std::cout << "Accept: " << this->Accept << std::endl;
  std::cout << "Accept Language: " << this->AcceptLanguage << std::endl;
  std::cout << "Accept Encoding: " << this->AcceptEncoding << std::endl;
  std::cout << "Connection: " << this->Connection << std::endl;
  std::cout << "Body: " << this->body << std::endl;
  std::cout << "Host Name: " << this->hostName << std::endl;
  std::cout << "Content Type: " << this->contentType << std::endl;
  std::cout << "Content Length: " << this->contentLength << std::endl;
  std::cout << "Boundary: " << this->boundary << std::endl;
  std::cout << "File Name: " << this->fileName << std::endl;
  std::cout << "File Content: " << this->fileContent << std::endl;
}