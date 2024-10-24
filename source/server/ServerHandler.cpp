/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:49 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/24 16:58:12 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerHandler.hpp"

using namespace std;

ServerHandler::~ServerHandler()
{
}

ServerHandler::ServerHandler(const ConfigFile& configFile)
{
    serversList = ConfigExtractor::extractServers(configFile);
}

void ServerHandler::displayServerHandlerInfo() const
{
    std::cout << "Server Handler Information:" << std::endl;
    for (size_t i = 0; i < serversList.size(); ++i) {
        // serversList[i].displayServerInfo();
    }
}

void ServerHandler::serversStart()
{
    for (size_t i = 0; i < serversList.size(); ++i) {
        serversList[i].start();
        addToEpoll(serversList[i].getListenFd());
    }
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
                // modif peut etre en dessous demain
                ClientIn client(currentFd, *this);
            } else {
                // normalement en dessous handleclientdata
                std::cout << "ok" << std::endl;
            }
        }
    }
}