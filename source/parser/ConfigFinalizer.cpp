#include "ConfigFinalizer.hpp"

ConfigFinalizer::~ConfigFinalizer() { }

ConfigFinalizer::ConfigFinalizer()
    : isValid(true)
    , defaultServerName("_")
    , defaultServerRoot("/www/")
    , defaultServerIndex("index.html")
    , defaultServerPort(80)
    , defaultServerClientMaxBodySize(1 * 1024 * 1024)
    , defaultLocationPath("/")
    , defaultLocationRoot("/www/")
{
}

ConfigFinalizer::ConfigFinalizer(const ConfigFinalizer& other)
{
    *this = other;
}

ConfigFinalizer& ConfigFinalizer::operator=(const ConfigFinalizer& other)
{
    if (this != &other) {
        this->isValid = other.isValid;
        this->defaultServerName = other.defaultServerName;
        this->defaultServerRoot = other.defaultServerRoot;
        this->defaultServerIndex = other.defaultServerIndex;
        this->defaultServerPort = other.defaultServerPort;
        this->defaultServerClientMaxBodySize = other.defaultServerClientMaxBodySize;
        this->defaultLocationPath = other.defaultLocationPath;
        this->defaultLocationRoot = other.defaultLocationRoot;
        this->ports = other.ports;
    }
    return *this;
}

bool ConfigFinalizer::good() const
{
    return this->isValid;
}

void ConfigFinalizer::finalizeConfig(std::vector<Server>& servers)
{
    std::vector<Server>::const_iterator itServer;
    for (itServer = servers.begin(); itServer != servers.end(); ++itServer) {
        int port = itServer->getPort();
        if (std::find(usedPorts.begin(), usedPorts.end(), port) == usedPorts.end()) {
            usedPorts.push_back(port);
        }
    }

    // Debug
    // std::cout << "Used Ports : ";
    // for (std::list<int>::const_iterator it = usedPorts.begin(); it != usedPorts.end(); ++it) {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;
}

void ConfigFinalizer::