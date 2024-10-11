/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigExtractor.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 08:10:43 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 10:24:00 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ConfigFile.hpp"
#include "Server.hpp"
#include <vector>

class ConfigExtractor {
public:
    // ~ConfigExtractor();
    // ConfigExtractor();

    static std::vector<Server> extractServers(const ConfigFile& configFile);
    static void extractServerDirectives(const Block& block, Server& server);
    static void extractLocationBlocks(const Block& block, Server& server);
    static void extractLocationDirectives(const Block& block, Location& location);

    // private:
    //     ConfigExtractor(const ConfigExtractor& other);
    //     ConfigExtractor& operator=(const ConfigExtractor& other);
};