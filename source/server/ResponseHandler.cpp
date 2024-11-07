/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:22:54 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/07 14:26:54 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseHandler.hpp"
#include <fstream>
#include <sstream>
#include <sys/socket.h> // For send()
#include <unistd.h> // For close()

ResponseHandler::ResponseHandler(Client& client, const RequestParsed& requestParsed)
    : client(client)
    , requestParsed(requestParsed)
{
    statusCode = static_cast<int>(requestParsed.status);
    reasonPhrase = getReasonPhrase(statusCode);
}

ResponseHandler::~ResponseHandler() { }

void ResponseHandler::handleResponse()
{
    buildResponse();
    sendResponse();
    closeConnectionIfNeeded();
}

void ResponseHandler::buildResponse()
{
    if (statusCode == 200) {
        handleSuccess();
    } else {
        handleError();
    }

    // Add necessary headers
    headerBuilder.addHeader("Content-Type", "text/html");

    // Update Content-Length header
    std::ostringstream oss;
    oss << body.size();
    headerBuilder.addHeader("Content-Length", oss.str());

    // Construct the full response
    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 " << statusCode << " " << reasonPhrase << "\r\n";
    responseStream << headerBuilder.buildHeaders();
    responseStream << "\r\n";
    responseStream << body;

    // Store the full response in 'body' for sending
    body = responseStream.str();
}

void ResponseHandler::handleSuccess()
{
    // Handle successful requests
    if (requestParsed.uri == "/") {
        // Serve the index page
        std::ifstream file("./index/index.html");
        if (file) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            body = buffer.str();
        } else {
            // If index file is not found, treat it as a 404 error
            statusCode = 404;
            reasonPhrase = getReasonPhrase(statusCode);
            loadErrorPage();
        }
    } else {
        // For other URIs, implement routing logic or return 404
        statusCode = 404;
        reasonPhrase = getReasonPhrase(statusCode);
        loadErrorPage();
    }
}

void ResponseHandler::handleError()
{
    // Load the error page corresponding to the current statusCode
    loadErrorPage();
}

void ResponseHandler::loadErrorPage()
{
    if (statusCode == 200) {
        // No error page needed for status code 200
        return;
    }

    std::ostringstream errorPagePath;
    errorPagePath << "./errorsPages/" << statusCode << ".html";
    std::ifstream file(errorPagePath.str().c_str());

    if (file) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        body = buffer.str();
    } else {
        // Fallback content if the error page is missing
        std::ostringstream bodyStream;
        bodyStream << "<html><body><h1>Error " << statusCode << " - " << reasonPhrase << "</h1></body></html>";
        body = bodyStream.str();
    }
}

void ResponseHandler::sendResponse()
{
    ssize_t bytesSent = send(client.getClientFd(), body.c_str(), body.size(), 0);
    if (bytesSent == -1) {
        // Handle send error if needed
    }
}

void ResponseHandler::closeConnectionIfNeeded()
{
    // Check the Connection header to decide whether to close the socket
    std::map<std::string, std::string>::const_iterator it = requestParsed.headers.find("Connection");
    if (it != requestParsed.headers.end() && it->second == "close") {
        // Set the flag in Client instead of closing the socket here
        client.setConnectionShouldClose(true);
    }
}

std::string ResponseHandler::getReasonPhrase(int code) const
{
    switch (code) {
    case 200:
        return "OK";
    case 400:
        return "Bad Request";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 411:
        return "Length Required";
    case 413:
        return "Payload Too Large";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    case 503:
        return "Service Unavailable";
    case 504:
        return "Gateway Timeout";
    case 505:
        return "HTTP Version Not Supported";
    default:
        return "Unknown Status";
    }
}
