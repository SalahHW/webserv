/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParsingRequest.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:03:29 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/10 15:41:08 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPARSINGREQUEST_HPP
#define HTTPPARSINGREQUEST_HPP

#include "HttpParsingException.hpp"
#include <cerrno>
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

struct HttpRequest {
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
    HttpStatusCode status;
};

class HttpParsing {
private:
    std::string request;
    struct HttpRequest requestParsed;
    // enum HttpStatusCode statusCode;
    int client_fd;

public:
    HttpParsing(std::string requestToParse, int client_fd);
    ~HttpParsing();
    HttpParsing(const HttpParsing& src);
    HttpParsing& operator=(const HttpParsing& src);
    const HttpRequest& getHttpRequest() const;
    bool parseRequestLine(std::string requestLine);
    bool parseHeaders(const std::string& headerLines);
    void parseHttpRequest();
    void showHttpRequest();
    void sendHttpError(HttpStatusCode statuscode) const;
    std::string trim(const std::string& str);
};

#endif