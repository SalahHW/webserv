#pragma once

#include "HttpStatusCode.hpp"
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

struct RequestParsed {
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
    HttpStatusCode statusCode;
};

class ParseRequest {
private:
    std::string request;
    RequestParsed requestParsed;

    void findAndParseRequestLine(std::string::size_type& headersStartPos);
    void findAndParseHeaders(std::string::size_type headersStartPos, std::string::size_type& bodyStartPos);
    void parseRequestBody(std::string::size_type bodyStartPos);
    std::string trim(const std::string& str) const;

public:
    ParseRequest(const std::string& request);
    ~ParseRequest();
    ParseRequest(const ParseRequest& src);
    ParseRequest& operator=(const ParseRequest& src);

    RequestParsed& getParsedRequest();

    void parseHttpRequest();
    void showHttpRequest() const;
};