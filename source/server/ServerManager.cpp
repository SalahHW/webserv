#include "ServerManager.hpp"

ServerManager::~ServerManager()
{
}

ServerManager::ServerManager()
{
}

ServerManager::ServerManager(std::map<int, Port> ports)
    : ports(ports)
    , isRunning(false)
{
    if (ports.empty())
    {
        std::cerr << "Error: No ports provided for server manager." << std::endl;
        return;
    }
}

ServerManager::ServerManager(const ServerManager& other)
{
    *this = other;
}

ServerManager& ServerManager::operator=(const ServerManager& other)
{
    if (this != &other) {
        this->ports = other.ports;
        this->isRunning = other.isRunning;
    }
    return *this;
}