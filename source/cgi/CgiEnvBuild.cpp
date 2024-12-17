/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiEnvBuild.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-den <rvan-den@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 09:43:19 by rvan-den          #+#    #+#             */
/*   Updated: 2024/12/12 09:43:20 by rvan-den         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

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

void CgiHandler::setScriptPath(const std::string &originScriptPath)
{
    scriptPath = originScriptPath;
}

const std::string &CgiHandler::genScriptPath() const
{
    return (this->scriptPath);
}

const std::string CgiHandler::genHttpUserAgent()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();

    for (; mapIt != request.headers.end(); ++mapIt)
    {
        if (mapIt->first == "User-Agent") {
            std::string userAgent = mapIt->second.c_str();
            return ("HTTP_USER_AGENT=" + userAgent);
        }
    }
    return ("HTTP_USER_AGENT=");
}

const std::string CgiHandler::genHttpAccept()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt)
    {
        if (mapIt->first == "Accept") {
            std::string accept = mapIt->second.c_str();
            return ("HTTP_ACCEPT=" + accept);
        }
    }
    return ("HTTP_ACCEPT=");
}

const std::string CgiHandler::genHttpAcceptLanguage()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt)
    {
        if (mapIt->first == "Accept-Language") {
            std::string acceptLanguage = mapIt->second.c_str();
            return ("HTTP_ACCEPT_LANGUAGE=" + acceptLanguage);
        }
    }
    return ("HTTP_ACCEPT_LANGUAGE=");
}

const std::string CgiHandler::genHttpAcceptEncoding()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt)
    {
        if (mapIt->first == "Accept-Encoding") {
            std::string acceptEncoding = mapIt->second.c_str();
            return ("HTTP_ACCEPT_ENCODING=" + acceptEncoding);
        }
    }
    return ("HTTP_ACCEPT_ENCODING=");
}

const std::string CgiHandler::genHttpConnexion()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt) {
        if (mapIt->first == "Connection") {
            std::string connection = mapIt->second.c_str();
            return ("HTTP_CONNECTION=" + connection);
        }
    }
    return ("HTTP_CONNECTION=");
}

const std::string CgiHandler::genHttpHost()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt)
    {
        if (mapIt->first == "Host") {
            std::string host = mapIt->second.c_str();
            return ("HTTP_HOST=" + host);
        }
    }
    return ("HTTP_HOST=");
}

const std::string CgiHandler::genHttpUpgradeInsecureRequests()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt) {
        if (mapIt->first == "Upgrade-Insecure-Requests") {
            std::string upgradeInsecureRequests = mapIt->second.c_str();
            return ("HTTP_UPGRADE_INSECURE_REQUESTS=" + upgradeInsecureRequests);
        }
    }
    return ("HTTP_UPGRADE_INSECURE_REQUESTS=");
}

const std::string CgiHandler::genHttpSecFetchDest()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt) {
        if (mapIt->first == "Sec-Fetch-Dest") {
            std::string secFetchDest = mapIt->second.c_str();
            return ("HTTP_SEC_FETCH_DEST=" + secFetchDest);
        }
    }
    return ("HTTP_SEC_FETCH_DEST=");
}

const std::string CgiHandler::genHttpSecFetchMode()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt) {
        if (mapIt->first == "Sec-Fetch-Mode") {
            std::string secFetchMode = mapIt->second.c_str();
            return ("HTTP_SEC_FETCH_MODE=" + secFetchMode);
        }
    }
    return ("HTTP_SEC_FETCH_MODE=");
}

const std::string CgiHandler::genHttpSecFetchSite()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt) {
        if (mapIt->first == "Sec-Fetch-Site") {
            std::string secFetchSite = mapIt->second.c_str();
            return ("HTTP_SEC_FETCH_SITE=" + secFetchSite);
        }
    }
    return ("HTTP_SEC_FETCH_SITE=");
}

const std::string CgiHandler::genHttpPriority()
{
    std::map<std::string, std::string>::iterator mapIt = request.headers.begin();
    for (; mapIt != request.headers.end(); ++mapIt) {
        if (mapIt->first == "Priority") {
            std::string priority = mapIt->second.c_str();
            return ("HTTP_PRIORITY=" + priority);
        }
    }
    return ("HTTP_PRIORITY=");
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
            return ("PATH_INFO=" + pathInfo);
        }
    }
    return ("PATH_INFO=");
}

const std::string CgiHandler::genContentLenght()
{
    // TEMPORARY ! FIX LATER
    // SHOULD GET CONTENT LENGHT VIA PAGE REQUEST (sortof...)
    const char* filePath = "/home/sickest-one/Travail/webserv/www/index/index.html";

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return ("0");
    }
    std::streampos size = file.tellg();
    return ("CONTENT_LENGHT=" + this->convertSizetToString(static_cast<size_t>(size)));
}

int CgiHandler::checkQueryStringPresence(std::string &uri)
{
    if (uri.find('?') != std::string::npos) {
        return (1);
    }
    return (0);
}

void CgiHandler::buildEnv()
{
    envVec.push_back(this->genRequestMethod());
    envVec.push_back(this->genPathInfo("http://serveur.org/cgi-bin/monscript.cgi/marecherche"));
    //envVec.push_back(this->genContentLenght());
    envVec.push_back(this->genServerProtocol());
    envVec.push_back(this->genQueryString());
    envVec.push_back(this->genHttpUserAgent());
    envVec.push_back(this->genHttpAccept());
    envVec.push_back(this->genHttpHost());
    envVec.push_back(this->genHttpAcceptLanguage());
    envVec.push_back(this->genHttpAcceptEncoding());
    envVec.push_back(this->genHttpConnexion());
    envVec.push_back(this->genHttpUpgradeInsecureRequests());
    envVec.push_back(this->genHttpSecFetchDest());
    envVec.push_back(this->genHttpSecFetchMode());
    envVec.push_back(this->genHttpSecFetchSite());
    envVec.push_back(this->genHttpPriority());
}

const std::vector<std::string>& CgiHandler::getEnvVec() const
{
    return (envVec);
}

void CgiHandler::cleanupEnvArray(const std::vector<std::string> &env, char **envArray)
{
    if (!envArray) {
        std::cerr << ENVARRAY_MALLOC_FAIL << std::endl;
        return;
    }
    for (size_t i = 0; i < env.size(); ++i)
        delete[] envArray[i];
    delete[] envArray;
}

char **CgiHandler::allocateEnvArray(const std::vector<std::string> &env)
{
    char **envArray = new char *[env.size() + 1];
    envArray[env.size()] = NULL;
    for (size_t i = 0; i < env.size(); ++i)
    {
        envArray[i] = new char[env[i].size() + 1];
        std::strcpy(envArray[i], env[i].c_str());
    }
    return (envArray);
}
