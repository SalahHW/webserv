/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:51:58 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/06 13:59:17 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::~HttpRequest() { }

HttpRequest::HttpRequest(std::string request)
    : request(request)
{
    parseHttpRequest();
}

HttpRequest::HttpRequest(const HttpRequest& src)
    : request(src.request)
    , requestParsed(src.requestParsed)
{
}

HttpRequest& HttpRequest::operator=(const HttpRequest& src)
{
    if (this != &src) {
        this->request = src.request;
        this->requestParsed = src.requestParsed;
    }
    return *this;
}

const RequestParsed& HttpRequest::getHttpRequest() const
{
    return this->requestParsed;
}

bool HttpRequest::parseRequestLine(const std::string requestLine)
{
    std::istringstream stream(requestLine);
    if (!(stream >> this->requestParsed.method >> this->requestParsed.uri >> this->requestParsed.version)) {
        this->requestParsed.status = BAD_REQUEST;
        return false;
    }
    if (this->requestParsed.method != "GET" && this->requestParsed.method != "POST") {
        this->requestParsed.status = NOT_IMPLEMENTED;
        return false;
    }
    if (this->requestParsed.version != "HTTP/1.1") {
        this->requestParsed.status = HTTP_VERSION_NOT_SUPPORTED;
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
// Dans parseHeaders modif le while
bool HttpRequest::parseHeaders(const std::string& headerLines)
{
    std::istringstream stream(headerLines);
    std::string line;
    while (std::getline(stream, line) && line != "\r") {
        if (line.empty() || line == "\n" || line == "\r\n")
            break;
        std::string::size_type pos = line.find(':');
        if (pos == std::string::npos) {
            this->requestParsed.status = BAD_REQUEST;
            return false;
        }
        std::string headerName = trim(line.substr(0, pos));
        std::string headerValue = trim(line.substr(pos + 1)); // used to be + 2
        if (!headerValue.empty() && headerValue[headerValue.size() - 1] == '\r') {
            headerValue.erase(headerValue.size() - 1);
        }
        this->requestParsed.headers[headerName] = headerValue;
    }
    return true;
}

bool HttpRequest::findAndParseRequestLine(std::string::size_type& headersStartPos)
{
    std::string::size_type requestLineEnd = this->request.find("\r\n");
    if (requestLineEnd == std::string::npos) {
        this->requestParsed.status = BAD_REQUEST;
        return false;
    }

    std::string requestLine = this->request.substr(0, requestLineEnd);

    if (!parseRequestLine(requestLine)) {
        return false;
    }

    headersStartPos = requestLineEnd + 2;

    return true;
}

bool HttpRequest::findAndParseHeaders(std::string::size_type headersStartPos, std::string::size_type& bodyStartPos)
{
    std::string::size_type headersEnd = this->request.find("\r\n\r\n", headersStartPos);
    if (headersEnd == std::string::npos) {
        this->requestParsed.status = BAD_REQUEST;
        return false;
    }

    std::string headers = this->request.substr(headersStartPos, headersEnd - headersStartPos);

    if (!parseHeaders(headers)) {
        return false;
    }

    bodyStartPos = headersEnd + 4;

    return true;
}

bool HttpRequest::parseRequestBody(std::string::size_type bodyStartPos)
{
    if (this->requestParsed.method == "POST") {
        if (this->requestParsed.headers.find("Content-Length") == this->requestParsed.headers.end()) {
            this->requestParsed.status = LENGTH_REQUIRED;
            return false;
        }

        std::istringstream iss(this->requestParsed.headers["Content-Length"]);
        size_t contentLength;
        if (!(iss >> contentLength)) {
            this->requestParsed.status = BAD_REQUEST;
            return false;
        }

        if (this->request.size() < bodyStartPos + contentLength) {
            this->requestParsed.status = BAD_REQUEST;
            return false;
        }

        this->requestParsed.body = this->request.substr(bodyStartPos, contentLength);
    } else {
        this->requestParsed.body = "";
    }

    return true;
}

void HttpRequest::parseHttpRequest()
{
    this->requestParsed.status = OK;

    std::string::size_type headersStartPos;
    if (!findAndParseRequestLine(headersStartPos)) {
        return;
    }

    std::string::size_type bodyStartPos;
    if (!findAndParseHeaders(headersStartPos, bodyStartPos)) {
        return;
    }

    if (!parseRequestBody(bodyStartPos)) {
        return;
    }
}

void HttpRequest::showHttpRequest()
{
    std::cout << "Method: " << this->requestParsed.method << std::endl;
    std::cout << "URI: " << requestParsed.uri << std::endl;
    std::cout << "Version: " << requestParsed.version << std::endl;
    std::cout << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::iterator it = requestParsed.headers.begin(); it != requestParsed.headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "Body: " << requestParsed.body << std::endl;
    std::cout << "Status: " << requestParsed.status << std::endl;
}