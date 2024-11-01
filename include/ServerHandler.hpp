/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:21:07 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/01 10:42:09 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Block.hpp"
#include "Client.hpp"
#include "ConfigExtractor.hpp"
#include "ConfigFile.hpp"
#include "EpollException.hpp"
#include "Location.hpp"
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
    void addServerToList();
    void updateServerList();
};