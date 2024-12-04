/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sickest_one <sickest_one@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:00:31 by rvan-den          #+#    #+#             */
/*   Updated: 2024/12/03 16:05:04 by sickest_one      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler() {
    //request.headers.insert("Host", "localhost:8080");
    //request.headers.insert("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:132.0) Gecko/20100101 Firefox/132.0");
    //request.headers.insert("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,/;q=0.8");
    //request.headers.insert("Accept-Language", "en-US,en;q=0.5");
    //request.headers.insert("Accept-Encoding", "gzip, deflate, br, zstd");
    //request.headers.insert("Connection", "keep-alive");
    //request.headers.insert("Upgrade-Insecure-Requests", "1");
    //request.headers.insert("Sec-Fetch-Dest", "document");
    //request.headers.insert("Sec-Fetch-Mode", "navigate");
    //request.headers.insert("Sec-Fetch-Site", "none");
    //request.headers.insert("Sec-Fetch-User", "?1");
    //request.headers.insert("Priority", "u=0, i");
    //request.body = "";

	request.method = "GET";
	request.uri = "http://example.com/cgi-bin/script.cgi?num1=2&num2=4";
	request.version = "HTTP/1.1";
}

CgiHandler::~CgiHandler()
{}

CgiHandler::CgiHandler(const CgiHandler &other) {
	(void)other;
}

const std::string CgiHandler::genQueryString() {
    std::string::const_iterator it = std::find(request.uri.begin(), request.uri.end(), '?');
    if (it != request.uri.end()) {
        size_t pos = it - request.uri.begin();
        std::string queryString = request.uri.substr(pos + 1).c_str();
        return ("QUERY_STRING=" + queryString);
    }
    return ("QUERY_STRING=");
}

const std::string CgiHandler::genRequestMethod() {
    const std::string requestMethod = request.method.c_str();
    return ("REQUEST_METHOD=" + requestMethod);
}

const std::string CgiHandler::genServerProtocol() {
    const std::string serverProtocol = request.version.c_str();
    return ("SERVER_PROTOCOL=" + serverProtocol);
}

const std::string CgiHandler::genScriptPath()
{
    // TEMPORARY ! FIX LATER
    return ("SCRIPT PATH");
}

const std::string CgiHandler::getPathInfo(const std::string &input)
{
     const std::string marker = "cgi-bin/";
    std::string pathInfo;

    std::string::size_type markerPos = input.find(marker);
    if (markerPos != std::string::npos) {
        std::string::size_type nextSlashPos = input.find('/', markerPos + marker.length());
        if (nextSlashPos != std::string::npos) {
            pathInfo = input.substr(nextSlashPos + 1);
        }
    }
    return (pathInfo);
}

size_t CgiHandler::genContentLenght()
{
    // TEMPORARY ! FIX LATER
    // SHOULD GET CONTENT LENGHT VIA PAGE REQUEST (sortof...)
    const char* filePath = "/home/sickest_one/Travail/webserv/www/index/index.html";

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return 1;
    }
    std::streampos size = file.tellg();
    return (static_cast<size_t>(size));
}

void    CgiHandler::printHeader()
{
}

// TO DO : BUILD ENV ARRAY, K.I.S.S
