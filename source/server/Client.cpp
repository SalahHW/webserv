/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:27 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/30 15:57:19 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int listen_sock_fd, ServerHandler server)
{
    try {
        CreateClientSock(listen_sock_fd);
        makeSocketNonBlocking();
        setSocketBufferSize(65536, 65536);
        server.addToEpoll(this->getClientFd());
    } catch (const ClientException& excp) {
        std::cerr << "Client error: " << excp.what() << std::endl;
    }
}

Client::~Client()
{
    // close(this->client_fd);
}

Client::Client(const Client& src)
    : clientFd(src.clientFd)
    , clientAddr(src.clientAddr)
    , clientLen(src.clientLen)
    , flags(src.flags)
{
}

Client& Client::operator=(const Client& src)
{
    this->clientFd = src.clientFd;
    return *this;
}

const int& Client::getClientFd() const { return this->clientFd; }

const struct sockaddr_in& Client::getClientAddr() const
{
    return this->clientAddr;
}

const socklen_t& Client::getClientLen() const { return this->clientLen; }

const int& Client::getFlags() const { return this->flags; }

void Client::CreateClientSock(int listen_sock_fd)
{
    this->clientLen = sizeof(this->clientAddr);
    this->clientFd = accept(
        listen_sock_fd, (struct sockaddr*)&this->clientAddr, &this->clientLen);
    if (this->clientFd == -1)
        throw ClientException("accept");
}

void Client::makeSocketNonBlocking()
{
    this->flags = fcntl(this->clientFd, F_GETFL, 0);
    if (flags == -1)
        throw ClientException("fcntl");
    flags |= O_NONBLOCK;
    if (fcntl(this->clientFd, F_SETFL, this->flags) == -1)
        throw ClientException("fcntl");
}

void Client::setSocketBufferSize(int recvBufSize, int sendBufSize) const
{
    if (setsockopt(this->clientFd, SOL_SOCKET, SO_RCVBUF, &recvBufSize,
            sizeof(recvBufSize))
        == -1)
        throw ClientException("setsockopt SO_RCVBUF");
    if (setsockopt(this->clientFd, SOL_SOCKET, SO_SNDBUF, &sendBufSize,
            sizeof(sendBufSize))
        == -1)
        throw ClientException("setsockopt SO_SNDBUF");
}