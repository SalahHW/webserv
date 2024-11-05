/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:38:36 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/05 15:13:52 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpParsingException.hpp"
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
    INTERNAL_SERVER_ERROR = 500,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504
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
    // enum HttpStatusCode statusCode;
    RequestParsed requestParsed;

public:
    HttpRequest(std::string request);
    ~HttpRequest();
    HttpRequest(const HttpRequest& src);
    HttpRequest& operator=(const HttpRequest& src);
    const RequestParsed& getHttpRequest() const;
    bool parseRequestLine(std::string requestLine);
    bool parseHeaders(const std::string& headerLines);
    void parseHttpRequest();
    void showHttpRequest();
    // void sendHttpError(HttpStatusCode statusCode) const;
    std::string trim(const std::string& str);
};