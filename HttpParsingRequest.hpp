#ifndef HTTPPARSINGREQUEST_HPP
#define HTTPPARSINGREQUEST_HPP

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>


enum HttpStatusCode {
    OK = 200,
    BAD_REQUEST = 400,
    LENGTH_REQUIRED = 411,
    INTERNAL_SERVER_ERROR = 500
};

struct HttpRequest {
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

class HttpParsing {
private:
    std::string request;
    struct HttpRequest requestParsed;

public:
    HttpParsing(std::string requestToParse);
    ~HttpParsing();
    HttpParsing(const HttpParsing& src);
    HttpParsing& operator=(const HttpParsing& src);
    void parseRequestLine(const std::string& requestLine,
        HttpRequest& httpRequest);
    void parseHeaders(const std::string& headerLines, HttpRequest& httpRequest);
    void parseHttpRequest(const std::string& rawRequest);
    void showHttpRequest();
};

#endif