/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:21:07 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/24 16:12:04 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Block.hpp"
#include "ClientIn.hpp"
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

    void displayServerHandlerInfo() const;
    void serversStart(); // demarre les serv
    void serversStop() const; // stop les serv

private:
    ServerHandler();
    ServerHandler(const ServerHandler& src);
    ServerHandler& operator=(const ServerHandler& src);

    void addServer(const Server& block);

    std::map<int, Server> serversList; // besoin absolut de cette pair int server en map

    int epollFd;
    int nbEvents;
    void epollInit();
    void startToListen();
    void addToEpoll(int fdToAdd) const;
};