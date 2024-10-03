/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientIn.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:27 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/06 16:02:28 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientIn.hpp"

ClientIn::ClientIn(int listen_sock_fd, EpollManagement epoll)
{
    try {
        CreateClientSock(listen_sock_fd);
        makeSocketNonBlocking();
        setSocketBufferSize(65536, 65536);
        epoll.addToEpoll(this->client_fd);
    } catch (const ClientException& excp) {
        std::cerr << "Client error: " << excp.what() << std::endl;
    }
}

ClientIn::~ClientIn()
{
    // close(this->client_fd);
}

ClientIn::ClientIn(const ClientIn& src)
    : client_fd(src.client_fd)
    , client_addr(src.client_addr)
    , client_len(src.client_len)
    , flags(src.flags)
{
}

ClientIn& ClientIn::operator=(const ClientIn& src)
{
    this->client_fd = src.client_fd;
    return *this;
}

const int& ClientIn::getClientFd() const { return this->client_fd; }

const struct sockaddr_in& ClientIn::getClientAddr() const
{
    return this->client_addr;
}

const socklen_t& ClientIn::getClientLen() const { return this->client_len; }

const int& ClientIn::getFlags() const { return this->flags; }

void ClientIn::CreateClientSock(int listen_sock_fd)
{
    this->client_len = sizeof(this->client_addr);
    this->client_fd = accept(
        listen_sock_fd, (struct sockaddr*)&this->client_addr, &this->client_len);
    if (this->client_fd == -1)
        throw ClientException("accept");
}

void ClientIn::makeSocketNonBlocking()
{
    this->flags = fcntl(this->client_fd, F_GETFL, 0);
    if (flags == -1)
        throw ClientException("fcntl");
    flags |= O_NONBLOCK;
    if (fcntl(this->client_fd, F_SETFL, this->flags) == -1)
        throw ClientException("fcntl");
}

void ClientIn::setSocketBufferSize(int recvBufSize, int sendBufSize) const
{
    if (setsockopt(this->client_fd, SOL_SOCKET, SO_RCVBUF, &recvBufSize,
            sizeof(recvBufSize))
        == -1)
        throw ClientException("setsockopt SO_RCVBUF");
    if (setsockopt(this->client_fd, SOL_SOCKET, SO_SNDBUF, &sendBufSize,
            sizeof(sendBufSize))
        == -1)
        throw ClientException("setsockopt SO_SNDBUF");
}