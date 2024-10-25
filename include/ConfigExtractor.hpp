/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigExtractor.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 08:10:43 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/25 09:28:04 by sbouheni         ###   ########.fr       */
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

    static std::map<int, Server> extractServers(const ConfigFile& configFile);
    static void extractServerDirectives(const Block& block, Server& server);
    static void extractLocationBlocks(const Block& block, Server& server);
    static void extractLocationDirectives(const Block& block, Location& location);

    // private:
    //     ConfigExtractor(const ConfigExtractor& other);
    //     ConfigExtractor& operator=(const ConfigExtractor& other);
};
