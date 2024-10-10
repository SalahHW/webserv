/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigExtractor.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 08:10:43 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 08:19:07 by sbouheni         ###   ########.fr       */
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

    // private:
    //     ConfigExtractor(const ConfigExtractor& other);
    //     ConfigExtractor& operator=(const ConfigExtractor& other);
};