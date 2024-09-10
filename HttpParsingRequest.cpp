/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParsingRequest.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:03:15 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/10 15:41:58 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpParsingRequest.hpp"

HttpParsing::HttpParsing(std::string requestToParse, int client_fd)
    : request(requestToParse)
    , client_fd(client_fd)
{
    try {
        showHttpRequest();
    } catch (const HttpParsingException& excp) {
        std::cerr << "Http Parsing error: " << excp.what() << std::endl;
    }
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

const HttpRequest& HttpParsing::getHttpRequest() const
{
    return this->requestParsed;
}

void HttpParsing::sendHttpError(HttpStatusCode statusCode) const
{
    std::string message;
    switch (statusCode) {
    case OK:
        message = "HTTP/1.1 200 OK";
        break;
    case BAD_REQUEST:
        message = "HTTP/1.1 400 Bad Request\r\n\r\n";
        break;
    case PAGE_NOT_FOUND:
        message = "HTTP/1.1 404 Page Not Found\r\n\r\n";
        break;
    case INTERNAL_SERVER_ERROR:
        message = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        break;
    case BAD_GATEWAY:
        message = "HTTP/1.1 502 Bad Gateway\r\n\r\n";
        break;
    case SERVICE_UNAVAILABLE:
        message = "HTTP/1.1 503 Service Unavailable\r\n\r\n";
        break;
    case GATEWAY_TIMEOUT:
        message = "HTTP/1.1 504 Gateway Timeout\r\n\r\n";
        break;
    default:
        message = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        break;
    }

    ssize_t bytesWritten = write(this->client_fd, message.c_str(), message.size());
    if (bytesWritten == -1) {
        std::cerr << "Error sending HTTP response: " << strerror(errno) << std::endl;
        // implemente un truc pour supp le client et tout ...
    }
    // shutdown(this->client_fd, SHUT_WR);
}

bool HttpParsing::parseRequestLine(std::string requestLine)
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

std::string HttpParsing::trim(const std::string& str)
{
    const std::string::size_type start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    const std::string::size_type end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

bool HttpParsing::parseHeaders(const std::string& headerLines)
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

void HttpParsing::parseHttpRequest()
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

void HttpParsing::showHttpRequest()
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
    sendHttpError(this->requestParsed.status);
}