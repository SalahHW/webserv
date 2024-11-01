/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:49 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/01 10:43:05 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerHandler.hpp"
#include "Server.hpp"

ServerHandler::~ServerHandler()
{
}

ServerHandler::ServerHandler(const ConfigFile& configFile)
{
    serversList = ConfigExtractor::extractServers(configFile);
    startToListen();
}

ServerHandler::ServerHandler(const ServerHandler& other)
    : serversList(other.serversList)
    , epollFd(other.epollFd)
    , nbEvents(other.nbEvents)
{
}

ServerHandler& ServerHandler::operator=(const ServerHandler& other)
{
    if (this != &other) {
        serversList = other.serversList;
        epollFd = other.epollFd;
        nbEvents = other.nbEvents;
    }
    return *this;
}

void ServerHandler::displayServerHandlerInfo() const
{
    std::map<int, Server>::const_iterator it;

    std::cout << "Server Handler Information:" << std::endl;
    for (it = serversList.begin(); it != serversList.end(); ++it) {
        it->second.displayServerInfo();
    }
}

void ServerHandler::serversStart()
{
    for (size_t i = 0; i < serversList.size(); ++i) {
        serversList[i].start();
        addToEpoll(serversList[i].getListenFd());
    }
    updateServerList();
}

void ServerHandler::updateServerList()
{
    std::map<int, Server> updatedServersList;
    std::map<int, Server>::iterator it;
    for (it = this->serversList.begin(); it != this->serversList.end(); ++it) {
        int newKey = it->second.getListenFd();
        updatedServersList.insert(std::make_pair(newKey, it->second));
    }
    this->serversList = updatedServersList;
}

void ServerHandler::epollInit()
{
    this->epollFd = epoll_create1(0);
    if (this->epollFd == -1) {
        throw EpollException("epoll_create1");
    }
}

void ServerHandler::addToEpoll(int fdToAdd) const
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fdToAdd;
    if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, fdToAdd, &event) == -1) {
        throw EpollException("epoll_ctl");
    }
}

void ServerHandler::startToListen()
{
    struct epoll_event events[MAX_EVENTS];
    this->epollInit();
    this->serversStart();
    std::cout << "----------STARTING TO LISTENING----------" << std::endl;
    while (1) {
        this->nbEvents = epoll_wait(this->epollFd, events, MAX_EVENTS, -1);
        if (this->nbEvents == -1) {
            throw EpollException("epoll_wait " + std::string(strerror(errno)));
        }
        for (int i = 0; i < this->nbEvents; i++) {
            int currentFd = events[i].data.fd;
            std::map<int, Server>::const_iterator it = this->serversList.find(currentFd);
            if (it != this->serversList.end()) {
                std::cout << "Client" << std::endl; // debug line
                Client client(currentFd);
                this->serversList[currentFd].addClientToServer(client);
                this->addToEpoll(client.getClientFd());
                // continue;
            } else {
                // normalement en dessous handleclientdata
                std::cout << "ok" << std::endl; // debug line
                sleep(1000);
            }
        }
    }
}
