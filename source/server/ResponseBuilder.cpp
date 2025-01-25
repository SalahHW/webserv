#include "ResponseBuilder.hpp"

#include "Response.hpp"

ResponseBuilder::~ResponseBuilder() {}

ResponseBuilder::ResponseBuilder(
    Request* request, Response& response,
    const std::map<std::string, VirtualHost>& virtualHosts,
    const std::string& defaultVirtualHostName)
    : request(request),
      response(response),
      statusCode(0),
      virtualHost(
          findMatchingVirtualHost(virtualHosts, defaultVirtualHostName)) {
  try {
    checkRequest();
    if (request->getMethod() == "POST") {
      treatAPost();
      return;
    }
    if (!findMatchingLocation()) {
      setStatusCode(404);
    }
    determinedPath = determinePath();
    std::cout << "determinedPath: " << determinedPath << std::endl;
    determineStatusCode();
    buildStatusLine();
    buildContentType();
    buildContentLength();
    buildLocation();
    buildAllow();
    buildRetryAfter();
    buildConnection();
    buildBytesSent();
    buildDate();
    buildFullHeader();
    buildBytesTotal();
    buildBody();
    buildFullResponse();
  } catch (const HttpException& e) {
    statusCode = e.getCode();
    buildErrorPage(statusCode);
    buildStatusLine();
    buildContentType();
    buildErrorContentLength();
    buildLocation();
    buildAllow();
    buildRetryAfter();
    buildConnection();
    buildBytesSent();
    buildDate();
    buildFullHeader();
    buildBytesTotal();
    if (!determinedPath.empty()) {
      buildBody();
    }
    buildFullResponse();
  }
}

void ResponseBuilder::treatAPost() {
  if (!findMatchingLocation()) {
    setStatusCode(404);
  }
}

void ResponseBuilder::buildErrorContentLength() {
  if (determinedPath.empty()) {
    size_t bodySize = response.getBody().size();
    response.setContentLength("Content-Length: " + to_string(bodySize));
    response.setTransferEncoding("");
  } else {
    size_t fileSize = getFileSize(determinedPath);
    if (fileSize > BUFFER) {
      response.setContentLength("");
      buildTransferEncoding();
    } else {
      response.setContentLength("Content-Length: " + to_string(fileSize));
      response.setTransferEncoding("");
    }
  }
}

void ResponseBuilder::checkRequest() {
  if (!request->getIsRequestGood()) {
    if (!request->getMethodGood()) {
      setStatusCode(405);
    }

    setStatusCode(400);
  }
}

void ResponseBuilder::setStatusCode(size_t code) {
  if (code >= 400) {
    statusCode = code;
    throw HttpException(code, getReasonPhraseForCode(code));
  }
  statusCode = code;
}

std::string ResponseBuilder::determinePath() {
  const std::string& locPath = matchingLocation.getPath();
  const std::string& rootDir = matchingLocation.getRootDirectory();
  const std::string& indexFile = matchingLocation.getIndexFile();
  bool autoIndex = matchingLocation.getAutoIndex();

  std::string uri = request->getUri();
  std::string remainder;
  if (uri.find(locPath) == 0) {
    remainder = uri.substr(locPath.size());
  } else {
    remainder = uri;
  }

  if (!remainder.empty() && remainder[0] != '/') {
    remainder = "/" + remainder;
  }

  std::string finalPath = rootDir;
  if (!rootDir.empty() && rootDir[rootDir.size() - 1] == '/') {
    finalPath = rootDir.substr(0, rootDir.size() - 1);
  }
  finalPath += remainder;

  struct stat info;
  if (stat(finalPath.c_str(), &info) == 0) {
    if (S_ISDIR(info.st_mode)) {
      if (!indexFile.empty()) {
        if (!finalPath.empty() && finalPath[finalPath.size() - 1] != '/') {
          finalPath += "/";
        }
        finalPath += indexFile;

        struct stat indexInfo;
        if (stat(finalPath.c_str(), &indexInfo) == 0) {
          return finalPath;
        } else {
          if (autoIndex) {
            finalPath.erase(finalPath.rfind(indexFile));
            return finalPath;
          } else {
            setStatusCode(404);
          }
        }
      } else {
        if (autoIndex) {
          return finalPath;
        } else {
          setStatusCode(403);
        }
      }
    } else {
      return finalPath;
    }
  } else {
    setStatusCode(404);
  }

  return finalPath;
}

const VirtualHost& ResponseBuilder::findMatchingVirtualHost(
    const std::map<std::string, VirtualHost>& virtualHosts,
    const std::string& defaultVirtualHostName) {
  std::cout << "TO FIND HOST = " << request->getHostName() << std::endl;
  std::map<std::string, VirtualHost>::const_iterator it =
      virtualHosts.find(request->getHostName());
  if (it != virtualHosts.end()) {
    return it->second;
  } else {
    std::cout << "Virtual host not found" << std::endl;
    std::cout << "Default virtual host: " << defaultVirtualHostName
              << std::endl;
    return virtualHosts.find(defaultVirtualHostName)->second;
  }
}

void ResponseBuilder::determineStatusCode() {
  if (!doesVhostExist() || !doesUriExist() || !isRessourceAvailable()) {
    setStatusCode(404);
  }
  if (!isMethodAccepted()) {
    setStatusCode(405);
  } else {
    statusCode = 200;
    return;
  }
}

bool ResponseBuilder::isRessourceAvailable() {
  std::ifstream file(determinedPath.c_str());
  if (file.is_open()) {
    file.close();
    return true;
  }
  return false;
}

bool ResponseBuilder::doesUriExist() {
  if (matchingLocation.getPath() == "") {
    return false;
  } else {
    return true;
  }
}

bool ResponseBuilder::doesVhostExist() {
  if (virtualHost.getName() == "") {
    return false;
  } else {
    return true;
  }
}

bool ResponseBuilder::isMethodAccepted() {
  if ((request->getMethod() == "GET" && matchingLocation.getGetAccepted()) ||
      (request->getMethod() == "POST" && matchingLocation.getPostAccepted()) ||
      (request->getMethod() == "DELETE" &&
       matchingLocation.getDeleteAccepted())) {
    return true;
  } else {
    return false;
  }
}

bool ResponseBuilder::findMatchingLocation() {
  const std::map<std::string, Location>& locations = virtualHost.getLocations();
  size_t longestMatch = 0;
  bool found = false;

  for (std::map<std::string, Location>::const_iterator it = locations.begin();
       it != locations.end(); ++it) {
    const std::string& locationPath = it->first;
    if (request->getUri().compare(0, locationPath.length(), locationPath) ==
            0 &&
        locationPath.length() > longestMatch) {
      longestMatch = locationPath.length();
      matchingLocation = it->second;
      found = true;
    }
  }

  std::cout << "matchingLocation: " << matchingLocation.getPath() << std::endl;
  return found;
}

void ResponseBuilder::buildErrorPage(size_t errorCode) {
  {
    std::map<size_t, std::string>::const_iterator it =
        virtualHost.getErrorPages().find(errorCode);

    if (it != virtualHost.getErrorPages().end()) {
      const std::string& errorPagePath = it->second;
      if (!errorPagePath.empty()) {
        struct stat info;
        if (stat(errorPagePath.c_str(), &info) == 0 && S_ISREG(info.st_mode)) {
          determinedPath = errorPagePath;
          return;
        }
      }
    }

    std::vector<char> responseChar;

    std::ostringstream oss;
    oss << errorCode;
    std::string errorCodeStr = oss.str();

    appendToVector(responseChar, "<html>\r\n");
    appendToVector(responseChar, "<head>\r\n");
    appendToVector(responseChar, "    <title>Error ");
    appendToVector(responseChar, "/title>\r\n");
    appendToVector(responseChar, "</head>\r\n");
    appendToVector(responseChar, "<body>\r\n");
    appendToVector(responseChar, "    <h1>Error " + errorCodeStr + "</h1>\r\n");
    appendToVector(responseChar,
                   "    <p>The requested page could not be "
                   "found.</p>\r\n");
    appendToVector(responseChar, "</body>\r\n");
    appendToVector(responseChar, "</html>");

    response.setBody(responseChar);
  }
}

void ResponseBuilder::appendToVector(std::vector<char>& vec,
                                     const std::string& str) {
  vec.insert(vec.end(), str.begin(), str.end());
}

const std::string& ResponseBuilder::getReasonPhraseForCode(size_t code) {
  switch (code) {
    case 200: {
      static const std::string ok = "OK";
      return ok;
    }
    case 400: {
      static const std::string br = "Bad Request";
      return br;
    }
    case 403: {
      static const std::string forb = "Forbidden";
      return forb;
    }
    case 404: {
      static const std::string nf = "Not Found";
      return nf;
    }
    case 405: {
      static const std::string ma = "Method Not Allowed";
      return ma;
    }
    case 500: {
      static const std::string ise = "Internal Server Error";
      return ise;
    }
    default: {
      static const std::string unk = "Unknown Error";
      return unk;
    }
  }
}

void ResponseBuilder::buildStatusLine() {
  std::string statusLine = "HTTP/1.1 " + to_string(statusCode) + " ";

  statusLine += getReasonPhraseForCode(statusCode);
  response.setStatusLine(statusLine);
}

void ResponseBuilder::buildDate() {
  std::time_t currentTime = std::time(NULL);
  std::tm* gmtTime = std::gmtime(&currentTime);
  char buffer[128];
  std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmtTime);
  response.setDate(std::string("Date: ") + buffer);
}

void ResponseBuilder::buildContentLength() {
  response.setContentLength("Content-Length: " +
                            to_string(getFileSize(determinedPath)));
  if (getFileSize(determinedPath) > BUFFER) {
    response.setContentLength("");
    buildTransferEncoding();
  }
}

void ResponseBuilder::buildTransferEncoding() {
  response.setTransferEncoding("Transfer-Encoding: chunked\r\n");
}

void ResponseBuilder::buildContentType() {
  if (determinedPath.empty()) {
    response.setContentType("Content-Type: text/html");
  } else {
    response.setContentType("Content-Type: " + findContentType(determinedPath));
    std::cout << "Content-Type: " << findContentType(determinedPath)
              << std::endl;
  }
}

const std::string ResponseBuilder::findContentType(
    const std::string& fileName) {
  std::string::size_type pos = fileName.rfind('.');
  if (pos == std::string::npos) {
    return "application/octet-stream";
  }

  std::string extension = fileName.substr(pos);

  for (std::size_t i = 0; i < extension.size(); i++) {
    extension[i] = static_cast<char>(tolower(extension[i]));
  }
  if (extension == ".html" || extension == ".htm") {
    return "text/html; charset=UTF-8";
  } else if (extension == ".css") {
    return "text/css";
  } else if (extension == ".js") {
    return "application/javascript";
  } else if (extension == ".jpg" || extension == ".jpeg") {
    return "image/jpeg";
  } else if (extension == ".png") {
    return "image/png";
  } else if (extension == ".json") {
    return "application/json";
  } else if (extension == ".xml") {
    return "application/xml";
  } else if (extension == ".pdf") {
    return "application/pdf";
  } else if (extension == ".zip") {
    return "application/zip";
  } else if (extension == ".tar") {
    return "application/x-tar";
  } else if (extension == ".gif") {
    return "image/gif";
  } else if (extension == ".bmp") {
    return "image/bmp";
  } else if (extension == ".ico") {
    return "image/x-icon";
  } else if (extension == ".mp3") {
    return "audio/mpeg";
  } else if (extension == ".mp4") {
    return "video/mp4";
  }
  return "application/octet-stream";
}

void ResponseBuilder::buildBody() {
  std::ifstream file(determinedPath.c_str(), std::ios::in | std::ios::binary);
  if (!file.is_open() && !determinedPath.empty()) {
    setStatusCode(404);
    // throw HttpException(404, "File not found");
  }

  std::size_t offset = response.getBytesLoad();
  if (offset > response.getFullHeader().size()) {
    response.clearForChunked();
  }
  file.seekg(offset, std::ios::beg);
  size_t bufferSize = BUFFER - response.getFullHeader().size() - 4;
  if ((!response.getTransferEncoding().empty()) || offset != 0) {
    bufferSize -= 3;
  }

  std::vector<char> buffer(bufferSize);
  file.read(&buffer[0], bufferSize);
  std::streamsize bytesRead = file.gcount();
  if (bytesRead <= 0) {
    if (file.eof()) {
      if (response.getContentLength().empty()) {
        std::vector<char> endChunkedBody = {'0', '\r', '\n', '\r', '\n'};
        response.setBody(endChunkedBody);
        response.setBytesLoad(response.getBytesTotal());
        response.setBytesSent(response.getBytesTotal() - endChunkedBody.size());
      }
    } else {
      throw HttpException(500, "Error reading file");
    }
    file.close();
    response.setFullResponse(response.getBody());
    return;
  }
  std::vector<char> content(buffer.begin(), buffer.begin() + bytesRead);
  if (!response.getContentLength().empty()) {
    response.setBody(content);
  } else {
    std::ostringstream oss;
    oss << std::hex << bytesRead << "\r\n";
    std::string chunkHeader = oss.str();
    std::vector<char> chunkBody(chunkHeader.begin(), chunkHeader.end());
    chunkBody.insert(chunkBody.end(), content.begin(), content.end());
    chunkBody.insert(chunkBody.end(), {'\r', '\n'});
  }
  response.setBytesLoad(response.getFullHeader().size() + offset + bytesRead);
  file.close();
  if (offset != 0) {
    response.setFullResponse(response.getFullHeader() + response.getBody());
  }
}

void ResponseBuilder::buildLocation() {}

void ResponseBuilder::buildAllow() {}

void ResponseBuilder::buildRetryAfter() {}

void ResponseBuilder::buildConnection() {
  // still dont know when it becomes "close"
  response.setConnection("Connection: keep-alive");
}

void ResponseBuilder::buildBytesSent() { response.setBytesSent(0); }

void ResponseBuilder::buildBytesTotal() {
  size_t total = getFileSize(determinedPath) + response.getFullHeader().size();
  if (!response.getTransferEncoding().empty()) {
    total += 5;
  }
  response.setBytesTotal(total);
}

void ResponseBuilder::buildFullHeader() {
  if (response.getContentLength().empty()) {
    response.setFullHeader(response.getStatusLine() + "\r\n" +
                           response.getContentType() + "\r\n" +
                           response.getTransferEncoding() + response.getDate() +
                           "\r\n" + response.getConnection() + "\r\n\r\n");
  } else {
    response.setFullHeader(response.getStatusLine() + "\r\n" +
                           response.getContentType() + "\r\n" +
                           response.getTransferEncoding() + response.getDate() +
                           "\r\n" + response.getContentLength() + "\r\n" +
                           response.getConnection() + "\r\n\r\n");
  }
}

void ResponseBuilder::buildFullResponse() {
  std::string fullResponse = response.getFullHeader() + response.getBody();
  response.setFullResponse(fullResponse);
}

const std::string ResponseBuilder::to_string(size_t value) {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

size_t ResponseBuilder::getFileSize(const std::string& filePath) {
  std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    std::cout << "File not found" << std::endl;
    if (response.getBody().size() != 0) {
      return response.getBody().size();
    }
    return 0;
  }
  file.seekg(0, std::ios::end);
  size_t size = static_cast<std::size_t>(file.tellg());
  return size;
}