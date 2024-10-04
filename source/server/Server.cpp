/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:42 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/04 10:31:36 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() { }

Server::Server(const Server& src) {
    (void)src;
 }

Server::~Server() { }

void Server::setListenFd(int fd)
{
    this->listenFd = fd;
}

void Server::setPort(int port)
{
    this->port = port;
}

void Server::setCLientMaxBodySize(int size)
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

int Server::getCLientMaxBodySize() const
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