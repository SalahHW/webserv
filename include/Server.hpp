/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:46 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/31 14:12:16 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"
#include "SocketException.hpp"
#include <cstdio> // pour sprintf
#include <fcntl.h> // Pour fcntl
#include <iostream> // Pour std::cerr
#include <map>
#include <netdb.h> // Pour getaddrinfo, freeaddrinfo, gai_strerror
#include <netinet/in.h> // Pour sockaddr_in, INADDR_ANY
#include <stdlib.h> // Pour exit
#include <string.h> // Pour memset, memcpy
#include <sys/socket.h> // Pour socket, bind, listen, accept
#include <unistd.h> // Pour close

class Server {
public:
    ~Server();
    Server();
    Server(const Server& src);
    Server& operator=(const Server& src);

    void setListenFd();
    void setPort(int port);
    void setClientMaxBodySize(int size);
    void setName(const std::string& name);
    void setErrorPages(std::map<int, std::string> errorPages);

    void addLocation(const Location& location);

    int getListenFd() const;
    int getPort() const;
    int getClientMaxBodySize() const;
    const std::string& getName() const;
    const std::map<int, std::string>& getErrorPages() const;

    void displayServerInfo() const;

    void start();

private:
    int listenFd;
    int port;
    std::string name;
    int clientMaxBodySize;
    std::map<int, std::string> errorPages;
    std::vector<Location> locations;
    struct sockaddr_in addr;
    void resolveHostName();
    void bindSocket() const;
    void setToListen() const;
    void makeSocketNonBlocking() const;
};