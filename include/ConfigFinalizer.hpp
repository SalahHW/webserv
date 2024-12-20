#pragma once

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Location.hpp"
#include "Port.hpp"
#include "Server.hpp"

class ConfigFinalizer {
public:
    ~ConfigFinalizer();
    ConfigFinalizer();
    ConfigFinalizer(const ConfigFinalizer& other);
    ConfigFinalizer& operator=(const ConfigFinalizer& other);

    bool good() const;
    void finalizeConfig(std::vector<Server>& servers);
    void finalizeServer(Server &server);
    void addUsedPort(int port);

private:
    // Default Server Values
    bool isValid;
    std::string defaultServerName;
    std::string defaultServerRoot;
    std::string defaultServerIndex;
    int defaultServerPort;
    int defaultServerClientMaxBodySize;

    // Default Location Values
    std::string defaultLocationPath;
    std::string defaultLocationRoot;

    std::list<int> usedPorts;

    std::vector<Port> ports;
};