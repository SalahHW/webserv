/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:22:54 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/13 17:13:41 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>

#include "ResponseHandler.hpp"
#include <iostream>
#include <sstream>
#include <sys/stat.h>

ResponseHandler::ResponseHandler(const RequestParsed& requestParsed)
    : requestParsed(requestParsed)
{
}

ResponseHandler::~ResponseHandler()
{
}

void ResponseHandler::handleResponse()
{
    buildResponse();
}

void ResponseHandler::buildResponse()
{
    int statusCode = requestParsed.status;

    if (statusCode == OK) {
        std::string filePath = determineFilePath();

        if (filePath.empty()) {
            statusCode = PAGE_NOT_FOUND;
            prepareErrorResponse(statusCode);
        } else {
            prepareSuccessResponse(filePath);
        }
    } else {
        prepareErrorResponse(statusCode);
    }

    assembleFullResponse(statusCode);
}

std::string ResponseHandler::determineFilePath() const
{
    std::string uri = requestParsed.uri;
    std::string filePath = getFilePathFromUri(uri);

    if (!fileExists(filePath)) {
        return ""; // Fichier introuvable
    }
    return filePath;
}

void ResponseHandler::prepareSuccessResponse(const std::string& filePath)
{
    body = readFile(filePath);

    if (body.empty()) {
        prepareErrorResponse(INTERNAL_SERVER_ERROR);
        return;
    }

    std::string contentType = getContentType(filePath);
    headerBuilder.addHeader("Content-Type", contentType);

    std::stringstream ss;
    ss << body.size();
    headerBuilder.addHeader("Content-Length", ss.str());
}

void ResponseHandler::prepareErrorResponse(int statusCode)
{
    std::stringstream errorFilePath;
    errorFilePath << "www/errorsPages/" << statusCode << ".html";
    body = readFile(errorFilePath.str());

    if (body.empty()) {
        body = "<html><body><h1>" + getReasonPhrase(statusCode) + "</h1></body></html>";
    }

    headerBuilder.setStatusCode(statusCode);
    headerBuilder.setReasonPhrase(getReasonPhrase(statusCode));
    headerBuilder.addHeader("Content-Type", "text/html");

    std::stringstream ss;
    ss << body.size();
    headerBuilder.addHeader("Content-Length", ss.str());
}

void ResponseHandler::assembleFullResponse(int statusCode)
{
    std::string headers = headerBuilder.buildHeaders();

    std::stringstream responseStream;
    responseStream << "HTTP/1.1 " << statusCode << " " << getReasonPhrase(statusCode) << "\r\n"
                   << headers << "\r\n"
                   << body;
    fullResponse = responseStream.str();
}

std::string ResponseHandler::getReasonPhrase(int code) const
{
    switch (code) {
    case OK:
        return "OK";
    case BAD_REQUEST:
        return "Bad Request";
    case PAGE_NOT_FOUND:
        return "Not Found";
    case METHOD_NOT_ALLOWED:
        return "Method Not Allowed";
    case LENGTH_REQUIRED:
        return "Length Required";
    case REQUEST_ENTITY_TOO_LARGE:
        return "Request Entity Too Large";
    case INTERNAL_SERVER_ERROR:
        return "Internal Server Error";
    case NOT_IMPLEMENTED:
        return "Not Implemented";
    case BAD_GATEWAY:
        return "Bad Gateway";
    case SERVICE_UNAVAILABLE:
        return "Service Unavailable";
    case GATEWAY_TIMEOUT:
        return "Gateway Timeout";
    case HTTP_VERSION_NOT_SUPPORTED:
        return "HTTP Version Not Supported";
    default:
        return "Internal Server Error";
    }
}

std::string ResponseHandler::getFilePathFromUri(const std::string& uri) const
{
    std::string rootDirectory = "www"; // Répertoire racine
    std::string filePath = (uri == "/") ? rootDirectory + "/index/index.html" : rootDirectory + uri;
    std::cout << "URI: " << uri << " resolved to file path: " << filePath << std::endl;
    return filePath;
}

bool ResponseHandler::fileExists(const std::string& filePath) const
{
    struct stat buffer;
    bool exists = (stat(filePath.c_str(), &buffer) == 0);
    std::cout << "Checking existence of file: " << filePath << " - Exists: " << exists << std::endl;
    return exists;
}

std::string ResponseHandler::getContentType(const std::string& filePath) const
{
    if (filePath.find(".html") != std::string::npos)
        return "text/html";
    else if (filePath.find(".css") != std::string::npos)
        return "text/css";
    else if (filePath.find(".js") != std::string::npos)
        return "application/javascript";
    else if (filePath.find(".png") != std::string::npos)
        return "image/png";
    else if (filePath.find(".jpg") != std::string::npos || filePath.find(".jpeg") != std::string::npos)
        return "image/jpeg";
    else
        return "application/octet-stream";
}

std::string ResponseHandler::readFile(const std::string& filePath) const
{
    std::ifstream file(filePath.c_str());
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

std::string ResponseHandler::getResponse() const
{
    return fullResponse;
}