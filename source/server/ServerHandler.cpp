/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:49 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/09 11:56:49 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerHandler.hpp"

using namespace std;

ServerHandler::~ServerHandler()
{
}

ServerHandler::ServerHandler(const ConfigFile& configFile)
{
    extractConfig(configFile);
}

void ServerHandler::extractConfig(const ConfigFile& configFile)
{
    const Block& rootBlock = configFile.getRootBlock();
    const std::vector<Block*>& subBlocks = rootBlock.getSubBlocks();

    for (size_t i = 0; i < subBlocks.size(); ++i) {
        const Block& block = *subBlocks[i];
        if (block.getName() == "server") {
            extractServerConfig(block);
        }
    }
}

void ServerHandler::extractServerConfig(const Block& block)
{
    Server server;

    const std::vector<Directive*> directives = block.getDirectives();
    for (size_t i = 0; i < directives.size(); ++i) {
        const Directive* directive = directives[i];
        (void)directive;

        // if (directive->getName() == "listen") {
        //     server.setPort(directive.getPort());
        // } else if (directive->getName() == "server_name") {
        //     server.setName(directive->getArguments()[0]);
        // } else if (directive->getName() == "client_max_body_size") {
        //     server.setClientMaxBodySize(atoi(directive->getArguments()[0].c_str()));
        //     // TODO: Handle error pages
        // }
    }

    const std::vector<Block*>& subBlocks = block.getSubBlocks();
    for (size_t j = 0; j < subBlocks.size(); ++j) {
        const Block& subBlock = *subBlocks[j];
        if (subBlock.getName() == "location") {
            // extractLocationConfig(server, subBlock);
        }
    }

    addServer(server);
}

void ServerHandler::extractLocationConfig(Server& server, const Block& block)
{
    Location location;

    const std::vector<Directive*> directives = block.getDirectives();
    for (size_t i = 0; i < directives.size(); ++i) {
        const Directive* directive = directives[i];
        if (directive->getName() == "root") {
            location.setRootDirectory(directive->getArguments()[0]);
        } else if (directive->getName() == "index") {
            location.setIndexFile(directive->getArguments()[0]);
        } else if (directive->getName() == "autoindex") {
            location.setAutoIndex(true);
        } else if (directive->getName() == "client_max_body_size") {
            location.setClientMaxBodySize(atoi(directive->getArguments()[0].c_str()));
        } else if (directive->getName() == "redirection") {
            location.setRedirectionPath(directive->getArguments()[0]);
        }
        // TODO: Handle accepted methods (GET, POST, DELETE)
    }
    server.addLocation(location);
}

void ServerHandler::addServer(const Server& block)
{
    this->serversList.push_back(block);
}

void ServerHandler::displayServerHandlerInfo() const
{
    std::cout << "Server Handler Information:" << std::endl;
    for (size_t i = 0; i < serversList.size(); ++i) {
        std::cout << "Server " << i + 1 << ":" << std::endl;
        serversList[i].displayServerInfo();
    }
}