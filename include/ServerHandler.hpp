/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:21:07 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/10 10:25:50 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Block.hpp"
#include "ConfigExtractor.hpp"
#include "ConfigFile.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <vector>

class Server;

class ServerHandler {
public:
    ~ServerHandler();
    ServerHandler(const ConfigFile& configFile);

    void displayServerHandlerInfo() const;

private:
    ServerHandler();
    ServerHandler(const ServerHandler& src);
    ServerHandler& operator=(const ServerHandler& src);
    
    void addServer(const Server& block);

    std::vector<Server> serversList;
};