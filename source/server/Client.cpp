/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:27 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/24 16:54:19 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "ServerHandler.hpp"

Client::Client(int listen_sock_fd)
{
    try {
        CreateClientSock(listen_sock_fd);
        makeSocketNonBlocking();
        setSocketBufferSize(65536, 65536);
        readRequest();
    } catch (const ClientException& excp) {
        std::cerr << "Client error: " << excp.what() << std::endl;
    }
}

Client::~Client()
{
    // close(this->client_fd);
}

Client::Client(const Client& src)
    : client_fd(src.client_fd)
    , client_addr(src.client_addr)
    , client_len(src.client_len)
    , flags(src.flags)
{
}

Client& Client::operator=(const Client& src)
{
    this->client_fd = src.client_fd;
    return *this;
}

const int& Client::getClientFd() const { return this->client_fd; }

const struct sockaddr_in& Client::getClientAddr() const
{
    return this->client_addr;
}

const socklen_t& Client::getClientLen() const { return this->client_len; }

const int& Client::getFlags() const { return this->flags; }

void Client::CreateClientSock(int listen_sock_fd)
{
    this->client_len = sizeof(this->client_addr);
    this->client_fd = accept(
        listen_sock_fd, (struct sockaddr*)&this->client_addr, &this->client_len);
    if (this->client_fd == -1)
        throw ClientException("accept");
}

void Client::makeSocketNonBlocking()
{
    this->flags = fcntl(this->client_fd, F_GETFL, 0);
    if (flags == -1)
        throw ClientException("fcntl");
    flags |= O_NONBLOCK;
    if (fcntl(this->client_fd, F_SETFL, this->flags) == -1)
        throw ClientException("fcntl");
}

void Client::setSocketBufferSize(int recvBufSize, int sendBufSize) const
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

void Client::readRequest()
{
    size_t count = 10000;
    std::string buffer(count, '\0');

    ssize_t bytes_read = read(this->client_fd, &buffer[0], count);
    // buffer.resize(bytes_read);
    (void)bytes_read;
    setRequest(buffer);
}

void Client::setRequest(std::string request)
{
    HttpRequest parser(request);
    this->request = parser.getHttpRequest();
    parser.showHttpRequest();
}