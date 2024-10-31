/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:32 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/24 16:51:40 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ClientException.hpp"
#include "ServerHandler.hpp"
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class ServerHandler;

class Client {
private:
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int flags;

public:
    Client(int listen_sock_fd, ServerHandler server);
    ~Client();
    Client(const Client& src);
    Client& operator=(const Client& src);
    const int& getClientFd() const;
    const struct sockaddr_in& getClientAddr() const;
    const socklen_t& getClientLen() const;
    const int& getFlags() const;
    void CreateClientSock(int listen_sock_fd);
    void makeSocketNonBlocking();
    void setSocketBufferSize(int recvBufSize, int sendBufSize) const;
};

#endif