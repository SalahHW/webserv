/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:49 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/14 15:16:20 by sbouheni         ###   ########.fr       */
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
    std::map<int, Server>::iterator it;
    for (it = serversList.begin(); it != serversList.end(); ++it) {
        addToEpoll(it->first);
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

void ServerHandler::handleClientData(int clientFd)
{
    char buffer[4096];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string data(buffer);

        // Find the client associated with this clientFd
        bool clientFound = false;
        for (std::map<int, Server>::iterator serverIt = this->serversList.begin(); serverIt != this->serversList.end(); ++serverIt) {
            Server& server = serverIt->second;
            std::map<int, Client>& clients = server.getClientsList();
            std::map<int, Client>::iterator clientIt = clients.find(clientFd);
            if (clientIt != clients.end()) {
                Client& client = clientIt->second;

                // Append data to client's request buffer
                client.appendToRequestBuffer(data);

                // If the connection should be closed, remove from epoll and close socket
                if (client.shouldCloseConnection()) {
                    epoll_ctl(this->epollFd, EPOLL_CTL_DEL, clientFd, NULL);
                    close(clientFd);
                    clients.erase(clientIt);
                }

                clientFound = true;
                break;
            }
        }
        if (!clientFound) {
            // Client not found, close connection
            epoll_ctl(this->epollFd, EPOLL_CTL_DEL, clientFd, NULL);
            close(clientFd);
        }
    } else if (bytesRead == 0) {
        // Client closed connection
        epoll_ctl(this->epollFd, EPOLL_CTL_DEL, clientFd, NULL);
        close(clientFd);

        // Remove client from server's client list
        for (std::map<int, Server>::iterator serverIt = this->serversList.begin(); serverIt != this->serversList.end(); ++serverIt) {
            Server& server = serverIt->second;
            std::map<int, Client>& clients = server.getClientsList();
            clients.erase(clientFd);
        }
    } else {
        // Error reading from client
        perror("recv");
        epoll_ctl(this->epollFd, EPOLL_CTL_DEL, clientFd, NULL);
        close(clientFd);

        // Remove client from server's client list
        for (std::map<int, Server>::iterator serverIt = this->serversList.begin(); serverIt != this->serversList.end(); ++serverIt) {
            Server& server = serverIt->second;
            std::map<int, Client>& clients = server.getClientsList();
            clients.erase(clientFd);
        }
    }
}

void ServerHandler::startToListen()
{
    struct epoll_event events[MAX_EVENTS];
    this->epollInit();
    this->serversStart();
    std::cout << "----------STARTING TO LISTEN----------" << std::endl;
    while (1) {
        this->nbEvents = epoll_wait(this->epollFd, events, MAX_EVENTS, -1);
        if (this->nbEvents == -1) {
            throw EpollException("epoll_wait " + std::string(strerror(errno)));
        }
        for (int i = 0; i < this->nbEvents; i++) {
            int currentFd = events[i].data.fd;
            std::map<int, Server>::const_iterator it = this->serversList.find(currentFd);
            if (it != this->serversList.end()) {
                // Accept new client connection
                Client client(currentFd);
                this->serversList[currentFd].addClientToServer(client);
                this->addToEpoll(client.getClientFd());
            } else {
                // Handle client data
                handleClientData(currentFd);
            }
        }
    }
}
