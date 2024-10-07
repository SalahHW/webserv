/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:42 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/07 12:09:50 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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

void Server::setListenFd(int fd)
{
    this->listenFd = fd;
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

void Server::setErrorPages(std::vector<std::string> error)
{
    this->errorPages = error;
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

const std::vector<std::string>& Server::getErrorPages() const
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
    
    std::cout << "- Error Pages: " << std::endl;
    for (size_t i = 0; i < errorPages.size(); ++i) {
        std::cout << "  * " << errorPages[i] << std::endl;
    }
    
    std::cout << "- Locations: " << std::endl;
    for (size_t i = 0; i < locations.size(); ++i) {
        locations[i].displayLocationInfo(); // Assumes Location class has a similar method to display its info
    }
}