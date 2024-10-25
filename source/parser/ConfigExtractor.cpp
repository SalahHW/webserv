/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigExtractor.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 08:10:24 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/25 10:46:46 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigExtractor.hpp"

// ConfigExtractor::~ConfigExtractor() { }

// ConfigExtractor::ConfigExtractor() { }

std::map<int, Server> ConfigExtractor::extractServers(const ConfigFile& configFile)
{
	std::map<int, Server> servers;
	const Block& mainBlock = configFile.getMainBlock();
	const std::vector<Block*> subBlocks = mainBlock.getSubBlocks();

	int serverCount = 0;

	for (size_t i = 0; i < subBlocks.size(); i++) {
		const Block& block = *subBlocks[i];
		if (block.getName() == "server") {
			Server server;
			extractServerDirectives(block, server);
			extractLocationBlocks(block, server);
			//TODO: use this one after bug fix
			// servers[server.getListenFd()] = server;
			servers[serverCount] = server;
			serverCount++;
		}
	}
	return servers;
}

void ConfigExtractor::extractServerDirectives(const Block& block, Server& server)
{
	const std::vector<Directive*> directives = block.getDirectives();
	for (size_t i = 0; i < directives.size(); ++i) {
		directives[i]->apply(server);
	}
}

void ConfigExtractor::extractLocationBlocks(const Block& block, Server& server)
{
	const std::vector<Block*>& subBlocks = block.getSubBlocks();
	for (size_t i = 0; i < subBlocks.size(); ++i) {
		const Block& subBlock = *subBlocks[i];
		if (subBlock.getName() == "location") {
			Location location;
			subBlock.apply(location);
			extractLocationDirectives(subBlock, location);
			server.addLocation(location);
		}
	}
}

void ConfigExtractor::extractLocationDirectives(const Block& block, Location& location)
{
	const std::vector<Directive*> directives = block.getDirectives();
	for (size_t i = 0; i < directives.size(); ++i) {
		directives[i]->apply(location);
	}
}
