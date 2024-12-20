/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:21:07 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/31 13:46:11 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Block.hpp"
#include "Client.hpp"
#include "ConfigExtractor.hpp"
#include "ConfigFile.hpp"
#include "EpollException.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <iostream>
#include <map>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 100

class Server;

class ServerHandler {
public:
    ~ServerHandler();
    ServerHandler(const ConfigFile& configFile);
    ServerHandler(const ServerHandler& src);
    ServerHandler& operator=(const ServerHandler& src);

    void displayServerHandlerInfo() const;
    void serversStart(); // demarre les serv
    void serversStop() const; // stop les serv
    void addToEpoll(int fdToAdd) const;

private:
    ServerHandler();

    void addServer(const Server& block);

    std::map<int, Server> serversList;

    int epollFd;
    int nbEvents;
    void epollInit();
    void startToListen();
};