/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerIn.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:03:44 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/10 14:21:28 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERIN_HPP
#define SERVERIN_HPP

#include "SocketException.hpp"
#include <cstdio>
#include <fcntl.h> // Pour fcntl
#include <iostream> // Pour std::cerr
#include <netdb.h> // Pour getaddrinfo, freeaddrinfo, gai_strerror
#include <netinet/in.h> // Pour sockaddr_in, INADDR_ANY
#include <stdlib.h> // Pour exit
#include <string.h> // Pour memset, memcpy
#include <sys/socket.h> // Pour socket, bind, listen, accept
#include <unistd.h> // Pour close

#define PORT 8080
#define HOSTNAME "localhost"

class ServerIn {
private:
    int listen_sock_fd;
    struct sockaddr_in addr;

public:
    ServerIn();
    ~ServerIn();
    ServerIn(const ServerIn& src);
    ServerIn& operator=(const ServerIn& src);
    void createSocket();
    void bindSocket() const;
    void makeSocketNonBlocking() const;
    void setToListen() const;
    const int& getListenSockFd() const;
    const struct sockaddr_in& getAddr() const;
    void resolveHostName();
};

#endif