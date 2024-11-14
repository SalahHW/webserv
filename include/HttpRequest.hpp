#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

enum HttpStatusCode {
    OK = 200,
    BAD_REQUEST = 400,
    PAGE_NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    LENGTH_REQUIRED = 411,
    REQUEST_ENTITY_TOO_LARGE = 413,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};

struct RequestParsed {
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
    HttpStatusCode status;
};

class HttpRequest {
private:
    std::string request;
    RequestParsed requestParsed;
    bool findAndParseRequestLine(std::string::size_type& headersStartPos);
    bool findAndParseHeaders(std::string::size_type headersStartPos, std::string::size_type& bodyStartPos);
    bool parseRequestBody(std::string::size_type bodyStartPos);

public:
    HttpRequest(std::string request);
    ~HttpRequest();
    HttpRequest(const HttpRequest& src);
    HttpRequest& operator=(const HttpRequest& src);
    const RequestParsed& getHttpRequest() const;
    bool parseRequestLine(const std::string requestLine);
    bool parseHeaders(const std::string& headerLines);
    void parseHttpRequest();
    void showHttpRequest();
    std::string trim(const std::string& str);
};