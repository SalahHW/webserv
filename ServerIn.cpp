/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIn.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:01:45 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/06 16:02:02 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerIn.hpp"

ServerIn::ServerIn()
{
    try {
        createSocket();
        bindSocket();
        makeSocketNonBlocking();
        setToListen();
    } catch (const SocketException& excp) {
        std::cerr << "Socket error: " << excp.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

ServerIn::~ServerIn()
{
    // close(this->listen_sock_fd);
}

ServerIn::ServerIn(const ServerIn& src)
    : listen_sock_fd(src.listen_sock_fd)
    , addr(src.addr)
{
}

ServerIn& ServerIn::operator=(const ServerIn& src)
{
    this->listen_sock_fd = src.listen_sock_fd;
    return *this;
}

const int& ServerIn::getListenSockFd() const { return this->listen_sock_fd; }

const struct sockaddr_in& ServerIn::getAddr() const
{
    return this->addr;
}

void ServerIn::createSocket()
{
    this->listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    if (this->listen_sock_fd == -1)
        throw SocketException("socket");
    if (setsockopt(this->listen_sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval,
            sizeof(optval))
        == -1)
        throw SocketException("setsockopt");
    memset(&this->addr, 0, sizeof(this->addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(PORT);
    this->addr.sin_addr.s_addr = INADDR_ANY;
}

void ServerIn::bindSocket() const
{
    if (bind(this->listen_sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        throw SocketException("bind");
}

void ServerIn::setToListen() const
{
    if (listen(this->listen_sock_fd, SOMAXCONN) == -1)
        throw SocketException("listen");
}

void ServerIn::makeSocketNonBlocking() const
{
    int flags = fcntl(this->listen_sock_fd, F_GETFL, 0);
    if (flags == -1)
        throw SocketException("fcntl");
    flags |= O_NONBLOCK;
    if (fcntl(this->listen_sock_fd, F_SETFL, flags) == -1)
        throw SocketException("fcntl");
}