/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:42 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/31 18:14:47 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Location.hpp"
#include "ServerHandler.hpp"

Server::~Server() { }

Server::Server() { }

Server::Server(const Server& src)
{
    *this = src;
}

Server& Server::operator=(const Server& src)
{
    if (this != &src) {
        this->listenFd = src.listenFd;
        this->port = src.port;
        this->name = src.name;
        this->clientMaxBodySize = src.clientMaxBodySize;
        this->errorPages = src.errorPages;
        this->locations = src.locations;
    }
    return *this;
}

void Server::setListenFd()
{
    resolveHostName();
    this->listenFd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    if (this->listenFd == -1)
        throw SocketException("socket");
    if (setsockopt(this->listenFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        throw SocketException("setsockopt");
    memset(&this->addr, 0, sizeof(this->addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->port);
    this->addr.sin_addr.s_addr = INADDR_ANY;
}

void Server::setPort(int port)
{
    this->port = port;
}

void Server::setClientMaxBodySize(int size)
{
    this->clientMaxBodySize = size;
}

void Server::setName(const std::string& name)
{
    this->name = name;
}

void Server::setErrorPages(std::map<int, std::string> errorPages)
{
    this->errorPages = errorPages;
}

void Server::addLocation(const Location& location)
{
    this->locations.push_back(location);
}

int Server::getListenFd() const
{
    return this->listenFd;
}

int Server::getPort() const
{
    return this->port;
}

int Server::getClientMaxBodySize() const
{
    return this->clientMaxBodySize;
}

const std::string& Server::getName() const
{
    return this->name;
}

const std::map<int, std::string>& Server::getErrorPages() const
{
    return this->errorPages;
}

void Server::displayServerInfo() const
{
    std::cout << "Server Information:" << std::endl;
    std::cout << "- Listen File Descriptor: " << listenFd << std::endl;
    std::cout << "- Port: " << port << std::endl;
    std::cout << "- Name: " << name << std::endl;
    std::cout << "- Client Max Body Size: " << clientMaxBodySize << " bytes" << std::endl;

    // std::cout << "- Error Pages: " << std::endl;
    // for (size_t i = 0; i < errorPages.size(); ++i) {
    //      std::cout << "  * " << errorPages[i] << std::endl;
    // }

    std::cout << "- Locations: " << std::endl;
    for (size_t i = 0; i < locations.size(); ++i) {
        locations[i].displayLocationInfo(); // Assumes Location class has a similar method to display its info
    }
}

void Server::resolveHostName()
{
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char port_str[6];
    sprintf(port_str, "%d", this->port);

    status = getaddrinfo(this->name.c_str(), port_str, &hints, &res);
    if (status != 0) {
        throw SocketException("getaddrinfo: " + std::string(gai_strerror(status)));
    }
    memcpy(&this->addr, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
}

void Server::bindSocket() const
{
    if (bind(this->listenFd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        throw SocketException("bind");
}

void Server::setToListen() const
{
    if (listen(this->listenFd, SOMAXCONN) == -1)
        throw SocketException("listen");
}

void Server::makeSocketNonBlocking() const
{
    int flags = fcntl(this->listenFd, F_GETFL, 0);
    if (flags == -1)
        throw SocketException("fcntl");
    flags |= O_NONBLOCK;
    if (fcntl(this->listenFd, F_SETFL, flags) == -1)
        throw SocketException("fcntl");
}

void Server::start()
{
    try {
        setListenFd();
        bindSocket();
        makeSocketNonBlocking();
        setToListen();
    } catch (const SocketException& excp) {
        std::cerr << "Socket error: " << excp.what() << std::endl;
    }
}

void Server::addClientToServer(Client clientToAdd)
{
    this->clientsList.insert(std::make_pair(clientToAdd.getClientFd(), clientToAdd));
}