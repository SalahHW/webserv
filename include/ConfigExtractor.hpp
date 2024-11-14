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
