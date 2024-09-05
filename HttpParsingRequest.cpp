#include "HttpParsingRequest.hpp"

HttpParsing::HttpParsing(std::string requestToParse)
    : request(requestToParse)
{
    showHttpRequest();
}

HttpParsing::~HttpParsing() { }

HttpParsing::HttpParsing(const HttpParsing& src)
    : request(src.request)
{
}

HttpParsing& HttpParsing::operator=(const HttpParsing& src)
{
    this->request = src.request;
    return *this;
}

void HttpParsing::parseRequestLine(const std::string& requestLine,
    HttpRequest& httpRequest)
{
    std::istringstream stream(requestLine);
    stream >> httpRequest.method;
    stream >> httpRequest.uri;
    stream >> httpRequest.version;
}

void HttpParsing::parseHeaders(const std::string& headerLines,
    HttpRequest& httpRequest)
{
    std::istringstream stream(headerLines);
    std::string line;
    while (std::getline(stream, line) && line != "\r") {
        std::string::size_type pos = line.find(':');
        if (pos != std::string::npos) {
            std::string headerName = line.substr(0, pos);
            std::string headerValue = line.substr(pos + 2);
            if (!headerValue.empty() && headerValue[headerValue.size() - 1] == '\r') {
                headerValue.erase(headerValue.size() - 1);
            }
            httpRequest.headers[headerName] = headerValue;
        }
    }
}

void HttpParsing::parseHttpRequest(const std::string& rawRequest)
{
    std::string::size_type pos = rawRequest.find("\r\n");
    if (pos != std::string::npos) {
        std::string requestLine = rawRequest.substr(0, pos);
        parseRequestLine(requestLine, this->requestParsed);
        std::string headerLines = rawRequest.substr(pos + 2, rawRequest.find("\r\n\r\n") - pos - 2);
        parseHeaders(headerLines, this->requestParsed);
        std::string::size_type bodyStart = rawRequest.find("\r\n\r\n");
        if (bodyStart != std::string::npos) {
            this->requestParsed.body = rawRequest.substr(bodyStart + 4);
        }
    }
}

void HttpParsing::showHttpRequest()
{
    parseHttpRequest(this->request);
    std::cout << "Method: " << this->requestParsed.method << std::endl;
    std::cout << "URI: " << requestParsed.uri << std::endl;
    std::cout << "Version: " << requestParsed.version << std::endl;
    std::cout << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::iterator it = requestParsed.headers.begin(); it != requestParsed.headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "Body: " << requestParsed.body << std::endl;
}