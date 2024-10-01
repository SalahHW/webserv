#include "Server.hpp"

Server::Server() { }

Server::Server(const Server& src) { }

Server::~Server() { }

Server& Server::operator=(const Server& src) { }

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
    this->ErrorPages = error;
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
    return this->ErrorPages;
}