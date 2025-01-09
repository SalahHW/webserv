#include "ResponseBuilder.hpp"

#include "Response.hpp"

ResponseBuilder::ResponseBuilder(
    const Request& request, Response& response,
    const std::map<std::string, VirtualHost>& virtualHosts)
    : request(request),
      response(response),
      statusCode(0),
      virtualHost(findMatchingVirtualHost(virtualHosts)) {
  buildStatusLine();
  buildTransferEncoding();
  buildContentType();
  buildBody();
  buildContentLength();
  buildLocation();
  buildAllow();
  buildRetryAfter();
  buildConnection();
  buildFullHeader();
  buildBytesSent();
  buildBytesTotal();
  buildDate();
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
  }
}

void ResponseBuilder::buildStatusLine() {}

void ResponseBuilder::buildDate() {
  std::time_t currentTime = std::time(NULL);
  std::tm* gmtTime = std::gmtime(&currentTime);
  char buffer[128];
  std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmtTime);
  response.setDate(std::string("Date: ") + buffer);
}

void ResponseBuilder::buildContentLength() {
  response.setContentLength(
      "Content-Length: " +
      to_string(response.getBody().size()));  // use getFileSizeinstead
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
}

void ResponseBuilder::buildBody() {}

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

const std::string& ResponseBuilder::buildFullResponse() {
  std::string fullResponse = response.getFullHeader() + response.getBody();
  return fullResponse;
}

const std::string to_string(size_t value) {
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