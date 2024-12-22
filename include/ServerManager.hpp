#pragma once

#include "Port.hpp"
#include <iostream>
#include <map>

class ServerManager {
public:
    ~ServerManager();
    ServerManager(std::map<int, Port> ports);
    ServerManager(const ServerManager& other);
    ServerManager& operator=(const ServerManager& other);

private:
    ServerManager();

    std::map<int, Port> ports;
    bool isRunning;
};