/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:51:58 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/05 15:14:29 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::~HttpRequest() { }

HttpRequest::HttpRequest(std::string request)
    : request(request)
{
}

HttpRequest::HttpRequest(const HttpRequest& src)
    : request(src.request)
{
}

HttpRequest& HttpRequest::operator=(const HttpRequest& src)
{
    this->request = src.request;
    return *this;
}

const RequestParsed& HttpRequest::getHttpRequest() const
{
    return this->requestParsed;
}

// void HttpRequest::sendHttpError(HttpStatusCode statusCode) const // a implementer dans Client
//  {
//  }

bool HttpRequest::parseRequestLine(std::string requestLine)
{
    std::istringstream stream(requestLine);
    if (!(stream >> this->requestParsed.method >> this->requestParsed.uri >> this->requestParsed.version)) {
        this->requestParsed.status = BAD_REQUEST;
        return false;
    }
    if (this->requestParsed.version != "HTTP/1.1") {
        this->requestParsed.status = BAD_REQUEST;
        return false;
    }
    return true;
}

std::string HttpRequest::trim(const std::string& str)
{
    const std::string::size_type start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    const std::string::size_type end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

bool HttpRequest::parseHeaders(const std::string& headerLines)
{
    std::istringstream stream(headerLines);
    std::string line;
    while (std::getline(stream, line) && line != "\r") {
        std::string::size_type pos = line.find(':');
        if (pos == std::string::npos) {
            this->requestParsed.status = BAD_REQUEST;
            return false;
        }
        std::string headerName = trim(line.substr(0, pos));
        std::string headerValue = trim(line.substr(pos + 2));
        if (!headerValue.empty() && headerValue[headerValue.size() - 1] == '\r') {
            headerValue.erase(headerValue.size() - 1);
        }
        this->requestParsed.headers[headerName] = headerValue;
    }
    return true;
}

void HttpRequest::parseHttpRequest()
{
    this->requestParsed.status = OK;
    std::string::size_type pos = this->request.find("\r\n");
    if (pos == std::string::npos) {
        throw HttpParsingException("Bad Request: No request line.");
    }
    if (!parseRequestLine(this->request.substr(0, pos))) {
        throw HttpParsingException("Bad Request: Invalid request line.");
    }
    if (!parseHeaders(this->request.substr(pos + 2, this->request.find("\r\n\r\n") - pos - 2))) {
        throw HttpParsingException("Bad Request: Invalid headers.");
    }
    if (this->requestParsed.headers.find("Content-Length") == this->requestParsed.headers.end() && this->requestParsed.method == "POST") {
        this->requestParsed.status = BAD_REQUEST;
        throw HttpParsingException("Bad Request, Content-Length missing for POST method.");
    }
    if (this->request.find("\r\n\r\n")) {
        this->requestParsed.body = this->request.substr(this->request.find("\r\n\r\n") + 4);
    }
}

void HttpRequest::showHttpRequest()
{
    parseHttpRequest();
    std::cout << "Method: " << this->requestParsed.method << std::endl;
    std::cout << "URI: " << requestParsed.uri << std::endl;
    std::cout << "Version: " << requestParsed.version << std::endl;
    std::cout << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::iterator it = requestParsed.headers.begin(); it != requestParsed.headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "Body: " << requestParsed.body << std::endl;
    // sendHttpError(this->requestParsed.status);
}