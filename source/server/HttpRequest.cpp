/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:25:00 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/14 12:25:00 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "HttpRequest.hpp"

HttpRequest::HttpRequest(const std::string& request)
    : request(request)
{
    parseHttpRequest();
}

HttpRequest::~HttpRequest()
{
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

void HttpRequest::parseHttpRequest()
{
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

void HttpRequest::showHttpRequest() const
{
    std::cout << "Method: " << this->requestParsed.method << std::endl;
    std::cout << "URI: " << this->requestParsed.uri << std::endl;
    std::cout << "Version: " << this->requestParsed.version << std::endl;
    std::cout << "Headers:" << std::endl;
    std::map<std::string, std::string>::const_iterator it;
    for (it = requestParsed.headers.begin(); it != requestParsed.headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "Body: " << this->requestParsed.body << std::endl;
    std::cout << "Status: " << this->requestParsed.status << std::endl;
}

std::string HttpRequest::trim(const std::string& str) const
{
    const std::string::size_type start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    const std::string::size_type end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

bool HttpRequest::findAndParseRequestLine(std::string::size_type& headersStartPos)
{
    std::string::size_type requestLineEnd = this->request.find("\r\n");
    if (requestLineEnd == std::string::npos) {
        this->requestParsed.status = BAD_REQUEST;
        return false;
    }

    std::string requestLine = this->request.substr(0, requestLineEnd);

    std::istringstream stream(requestLine);
    if (!(stream >> this->requestParsed.method >> this->requestParsed.uri >> this->requestParsed.version)) {
        this->requestParsed.status = BAD_REQUEST;
        return false;
    }

    if (!isMethodSupported(this->requestParsed.method)) {
        this->requestParsed.status = METHOD_NOT_ALLOWED;
        return false;
    }

    if (this->requestParsed.version != Constants::HTTP_VERSION) {
        this->requestParsed.status = HTTP_VERSION_NOT_SUPPORTED;
        return false;
    }

    this->requestParsed.status = OK;
    headersStartPos = requestLineEnd + 2;

    return true;
}

bool HttpRequest::findAndParseHeaders(std::string::size_type headersStartPos, std::string::size_type& bodyStartPos)
{
    std::string::size_type headersEnd = this->request.find(Constants::REQUEST_TERMINATOR, headersStartPos);
    if (headersEnd == std::string::npos) {
        this->requestParsed.status = BAD_REQUEST;
        return false;
    }

    std::string headers = this->request.substr(headersStartPos, headersEnd - headersStartPos);

    std::istringstream stream(headers);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }

        if (line.empty())
            continue;

        std::string::size_type pos = line.find(':');
        if (pos == std::string::npos) {
            this->requestParsed.status = BAD_REQUEST;
            return false;
        }

        std::string headerName = trim(line.substr(0, pos));
        std::string headerValue = trim(line.substr(pos + 1));

        this->requestParsed.headers[headerName] = headerValue;
    }

    bodyStartPos = headersEnd + Constants::REQUEST_TERMINATOR.size();

    return true;
}

bool HttpRequest::parseRequestBody(std::string::size_type bodyStartPos)
{
    if (this->requestParsed.method == "POST") {
        std::map<std::string, std::string>::const_iterator it = this->requestParsed.headers.find("Content-Length");
        if (it == this->requestParsed.headers.end()) {
            this->requestParsed.status = LENGTH_REQUIRED;
            return false;
        }

        std::istringstream iss(it->second);
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

bool HttpRequest::isMethodSupported(const std::string& method) const
{
    for (size_t i = 0; i < Constants::SUPPORTED_METHODS_COUNT; ++i) {
        if (method == Constants::SUPPORTED_METHODS[i]) {
            return true;
        }
    }
    return false;
}