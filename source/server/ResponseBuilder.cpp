#include "ResponseBuilder.hpp"

#include "Response.hpp"

ResponseBuilder::ResponseBuilder(
    const Request& request, Response& response,
    const std::map<std::string, VirtualHost>& virtualHosts)
    : request(request),
      response(response),
      statusCode(0),
      virtualHost(findMatchingVirtualHost(virtualHosts)) {
  findMatchingLocation();
  determinePath =
      matchingLocation.getRootDirectory() +
      matchingLocation.getIndexFile();  // trim the point before the indexfile
  determineStatusCode();
  buildStatusLine();
  buildContentType();
  buildBody();
  buildContentLength();
  buildLocation();
  buildAllow();
  buildRetryAfter();
  buildConnection();
  buildBytesSent();
  buildBytesTotal();
  buildDate();
  buildFullHeader();
  buildFullResponse();
}

ResponseBuilder::~ResponseBuilder() {}

const VirtualHost& ResponseBuilder::findMatchingVirtualHost(
    const std::map<std::string, VirtualHost>& virtualHosts) {
  std::map<std::string, VirtualHost>::const_iterator it =
      virtualHosts.find(request.getHost());
  if (it != virtualHosts.end()) {
    return it->second;
  } else {
    return virtualHosts.begin()
        ->second;  // I assume that the first virtual host is
                   //  the default one but maybe it's not since each port have a
                   //  list of virtual hosts
  }
}

void ResponseBuilder::determineStatusCode() {
  if (!request.getIsRequestGood()) {
    statusCode = 400;
    return;
  }
  if (!doesVhostExist() || !doesUriExist() || !isRessourceAvailable()) {
    statusCode = 404;
    return;
  }
  if (!isMethodAccepted()) {
    statusCode = 405;
    return;
  } else {
    statusCode = 200;
    return;
  }
}

bool ResponseBuilder::isRessourceAvailable() {
  std::ifstream file(determinePath.c_str());
  std::cout << "DETERMINE PATH= " << determinePath << std::endl;

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
  if ((request.getMethod() == "GET" && matchingLocation.getGetAccepted()) ||
      (request.getMethod() == "POST" && matchingLocation.getPostAccepted()) ||
      (request.getMethod() == "DELETE" &&
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
    if (request.getUri().compare(0, locationPath.length(), locationPath) == 0 &&
        locationPath.length() > longestMatch) {
      longestMatch = locationPath.length();
      matchingLocation = it->second;
      found = true;
    }
  }

  return found;
}

void ResponseBuilder::buildStatusLine() {
  std::string statusLine =
      "Status Line: HTTP/1.1 " + to_string(statusCode) + " ";
  switch (statusCode) {
    case 200:
      statusLine += "OK";
      break;
    case 400:
      statusLine += "Bad Request";
      break;
    case 404:
      statusLine += "Not Found";
      break;
    case 405:
      statusLine += "Method Not Allowed";
      break;
    default:
      statusLine += "Internal Server Error";
      break;
  }
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
                            to_string(getFileSize(request.getUri())));
  if (getFileSize(request.getUri()) > 1024) {
    response.setContentLength("");
    buildTransferEncoding();
  }
}

void ResponseBuilder::buildTransferEncoding() {
  response.setTransferEncoding("Transfer-Encoding: chunked");
}

void ResponseBuilder::buildContentType() {
  response.setContentType("Content-Type: " + findContentType(request.getUri()));
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
    return "text/html";
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
  std::ifstream file(determinePath.c_str(), std::ios::binary);
  if (!file.is_open()) {
    response.setBody("");
    return;
  }
  size_t readSize = 0;
  std::string prefix;
  if (response.getBytesLoad() != 0) {
    response.setBody("");
  }
  if (response.getBytesLoad() == 0) {
    prefix = "Body: ";
    if (prefix.size() < 1024) {
      readSize = 1024 - prefix.size();
    } else {
      readSize = 0;
    }
  }
  char buffer[1024];
  file.read(buffer, 1024);
  std::streamsize bytesRead = file.gcount();
  std::string content =
      prefix + std::string(buffer, static_cast<size_t>(bytesRead));
  if (bytesRead <= 0) {
    response.setBody("");
    file.close();
    return;
  }
  if (!response.getContentLength().empty()) {
    response.setBody(content);
    response.setBytesLoad(response.getBytesLoad() + bytesRead);
  } else {
    std::ostringstream oss;
    oss << std::hex << content.size() << "\r\n" << content << "\r\n";
    response.setBody(oss.str());
    response.setBytesLoad(response.getBytesLoad() + bytesRead + 2);
  }
  file.close();
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
  response.setBytesTotal(
      response.getFullHeader().size() +
      getFileSize(
          request.getUri()));  // uri in request need to be trimmed or
                               // just redefine in function of location add this
                               // funtion in RequestParser maybe
}

void ResponseBuilder::buildFullHeader() {
  response.setFullHeader(
      response.getStatusLine() + "\r\n" + response.getContentType() + "\r\n" +
      response.getContentLength() + "\r\n" + response.getTransferEncoding() +
      "\r\n" + response.getDate() + "\r\n" + response.getConnection() +
      "\r\n\r\n");
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
  std::ifstream file(filePath.c_str(), std::ios::binary);
  if (!file.is_open()) {
    return 0;
  }
  file.seekg(0, std::ios::end);
  size_t size = static_cast<std::size_t>(file.tellg());
  return size;
}