/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:27 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/13 15:00:00 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "ServerHandler.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

Client::Client(int client_fd)
    : client_fd(client_fd)
    , connectionShouldClose(false)
    , bytesSent(0)
{
}

Client::~Client()
{
    closeClientSocket();
}

Client::Client(const Client& other)
{
    copyClientData(other);
    std::cout << "Client copié avec le descripteur : " << client_fd << std::endl;
}

Client& Client::operator=(const Client& other)
{
    if (this != &other) {
        copyClientData(other);
    }
    return *this;
}

void Client::copyClientData(const Client& other)
{
    client_fd = other.client_fd;
    connectionShouldClose = other.connectionShouldClose;
    requestBuffer = other.requestBuffer;
    responseBuffer = other.responseBuffer;
    bytesSent = other.bytesSent;
    request = other.request;
}

void Client::closeClientSocket()
{
    if (client_fd != -1) {
        close(client_fd);
        client_fd = -1;
    }
}

const int& Client::getClientFd() const { return client_fd; }
bool Client::shouldCloseConnection() const { return connectionShouldClose; }
void Client::setConnectionShouldClose(bool shouldClose) { connectionShouldClose = shouldClose; }

void Client::appendToRequestBuffer(const std::string& data)
{
    requestBuffer += data;
    if (isRequestComplete()) {
        processRequest();
        requestBuffer.clear();
    }
}

bool Client::isRequestComplete() const
{
    return requestBuffer.find(REQUEST_TERMINATOR) != std::string::npos;
}

void Client::processRequest()
{
    parseRequest();
    handleResponse();
    prepareForSending();
}

void Client::parseRequest()
{
    HttpRequest parser(requestBuffer);
    request = parser.getHttpRequest();
    connectionShouldClose = false;
}

void Client::handleResponse()
{
    ResponseHandler responseHandler(request);
    responseHandler.handleResponse();
    setResponse(responseHandler.getResponse());
}

void Client::prepareForSending()
{
    bytesSent = 0;
    checkConnectionPersistence();
}

void Client::setResponse(const std::string& response)
{
    responseBuffer = response;
    std::cout << "Réponse configurée : " << responseBuffer << std::endl;
}

void Client::checkConnectionPersistence()
{
    std::map<std::string, std::string>::const_iterator it = request.headers.find(CONNECTION_HEADER);
    if (it != request.headers.end() && it->second == KEEP_ALIVE && request.version == HTTP_VERSION) {
        setConnectionShouldClose(false);
    } else {
        setConnectionShouldClose(true);
    }
}

bool Client::hasDataToWrite() const
{
    return bytesSent < responseBuffer.size();
}

ssize_t Client::sendData()
{
    ssize_t result = send(client_fd, responseBuffer.c_str() + bytesSent, responseBuffer.size() - bytesSent, 0);
    if (result > 0) {
        bytesSent += result;
        std::cout << "Envoyé " << result << " octets au client " << client_fd << std::endl;
    } else if (result == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
        handleError("send");
    }
    return result;
}

void Client::handleError(const std::string& functionName)
{
    std::cerr << "Erreur dans " << functionName << ": " << strerror(errno) << std::endl;
}
