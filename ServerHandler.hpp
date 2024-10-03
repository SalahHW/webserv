/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:21:07 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/03 13:24:25 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include "Server.hpp"
#include <iostream>
#include <map>
#include <string>

class Server;

class ServerHandler {
private:
    std::map<int, Server> serverList;
    ServerHandler(const ServerHandler& src);
    ServerHandler& operator=(const ServerHandler& src);

public:
    ServerHandler();
    ~ServerHandler();
    void addServer(int serverFd, const Server& src);
};

#endif