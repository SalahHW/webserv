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

    //AUTH_TYPE
    //CONTENT_LENGTH
    //CONTENT_TYPE
    //GATEWAY_INTERFACE
    //HTTP_*
    //PATH_INFO
    //PATH_TRANSLATED
    //QUERY_STRING
    //REMOTE_ADDR
    //REMOTE_HOST
    //REMOTE_IDENT
    //REMOTE_USER
    //REQUEST_METHOD
    //SCRIPT_NAME
    //SERVER_NAME
    //SERVER_PORT
    //SERVER_PROTOCOL
    //SERVER_SOFTWARE

#include "CgiHandler.hpp"

CgiHandler::CgiHandler() {
	request.method = "GET";
	request.uri = "http://example.com/cgi-bin/script.cgi?num1=2&num2=4";
	request.version = "HTTP/1.1";
}

CgiHandler::~CgiHandler()
{}

CgiHandler::CgiHandler(const CgiHandler &other) {
	(void)other;
}

const std::string CgiHandler::convertSizetToString(size_t value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

const std::string CgiHandler::genQueryString() {
    std::string::const_iterator it = std::find(request.uri.begin(), request.uri.end(), '?');
    if (it != request.uri.end()) {
        size_t pos = it - request.uri.begin();
        std::string queryString = request.uri.substr(pos + 1).c_str();
        return ("QUERY_STRING=" + queryString);
    }
    return ("");
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
    return ("SCRIPT PATH=");
}

const std::string CgiHandler::genHttpUserAgent()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();

    for (; mapIt != request.headers.end(); ++mapIt)
    {
        if (mapIt->first == "")
    }
    return ("");
}

const std::string CgiHandler::genPathInfo(const std::string &input)
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
    return ("PATH_INFO=" + pathInfo);
}

const std::string CgiHandler::genContentLenght()
{
    // TEMPORARY ! FIX LATER
    // SHOULD GET CONTENT LENGHT VIA PAGE REQUEST (sortof...)
    const char* filePath = "/home/sickest-one/Travail/webserv/www/index/index.html";

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return ("");
    }
    std::streampos size = file.tellg();
    return ("CONTENT_LENGHT=" + this->convertSizetToString(static_cast<size_t>(size)));
}

std::vector<std::string> CgiHandler::buildEnv() {
    // NOT DONE, NEED MORE DETAILS NIGGA
    std::vector<std::string> env;
    std::string query = this->genQueryString();

    env.push_back(this->genRequestMethod());
    // NOT CLEAN
    env.push_back(this->genPathInfo("http://serveur.org/cgi-bin/monscript.cgi/marecherche"));
    env.push_back(this->genContentLenght());
    env.push_back(this->genServerProtocol());
    if (!query.empty())
        env.push_back(query);

    // !!! Try using putenv () --> cstdlib
    return (env);
}

void CgiHandler::printEnv(std::vector<std::string> &env)
{
    // FOR DEBUG, DELETE LATER
    std::vector<std::string>::iterator vec_it;
    for (vec_it = env.begin(); vec_it != env.end(); vec_it++) {
        std::cout << *vec_it << std::endl;
    }
}
