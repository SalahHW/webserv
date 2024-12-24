#pragma once

#include <map>
#include <vector>

#include "ConfigFile.hpp"
#include "ConfigFinalizer.hpp"
#include "Port.hpp"
#include "Server.hpp"

class ConfigFile;

class ConfigExtractor {
public:
    static std::vector<Server> extractServers(const ConfigFile& configFile);
    static void extractServerDirectives(const Block& block, Server& server);
    static void extractLocationBlocks(const Block& block, Server& server);
    static void extractLocationDirectives(const Block& block, Location& location);
};
