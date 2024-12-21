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

bool ConfigFinalizer::good() const { return this->isValid; }

const std::map<int, Port>& ConfigFinalizer::getPorts() const { return ports; }

void ConfigFinalizer::finalizeConfig(std::vector<Server>& servers)
{
    std::vector<Server>::iterator itServer;
    for (itServer = servers.begin(); itServer != servers.end(); ++itServer) {
        finalizeServer(*itServer);
        finalizeLocations(*itServer);
        addUsedPort(itServer->getPort());
    }
    createPorts();
    assignServersToPorts(servers);
}

void ConfigFinalizer::finalizeServer(Server& server)
{
    if (!server.isPortDefined()) {
        server.setPort(defaultServerPort);
    }
    if (!server.isNameDefined()) {
        server.setName(defaultServerName);
    }
    if (!server.isClientMaxBodySizeDefined()) {
        server.setClientMaxBodySize(defaultServerClientMaxBodySize);
    }
    // TODO: Complete default Server configuration
}

void ConfigFinalizer::finalizeLocations(Server& server)
{
    std::map<std::string, Location>& locations = server.getLocations();
    std::map<std::string, Location>::iterator itLocation;

    for (itLocation = locations.begin(); itLocation != locations.end(); ++itLocation) {
        Location& location = itLocation->second;

        if (!location.isPathDefined()) {
            location.setPath(defaultLocationPath);
        }
        if (!location.isRootDirectoryDefined()) {
            location.setRootDirectory(defaultLocationRoot);
        }
        if (!location.isIndexFileDefined()) {
            location.setIndexFile(defaultServerIndex);
        }
        if (!location.isClientMaxBodySizeDefined()) {
            location.setClientMaxBodySize(server.getClientMaxBodySize());
        }
        // TODO: Complete default Location configuration
    }
}

void ConfigFinalizer::addUsedPort(int port)
{
    if (std::find(usedPorts.begin(), usedPorts.end(), port) == usedPorts.end()) {
        usedPorts.push_back(port);
    }
}

void ConfigFinalizer::createPorts()
{
    std::list<int>::const_iterator itUsedPorts;

    for (itUsedPorts = usedPorts.begin(); itUsedPorts != usedPorts.end(); ++itUsedPorts) {
        Port port;
        port.setPort(*itUsedPorts);
        ports[*itUsedPorts] = port;
    }
}

void ConfigFinalizer::assignServersToPorts(const std::vector<Server>& servers)
{
    std::vector<Server>::const_iterator itServers;

    for (itServers = servers.begin(); itServers != servers.end(); ++itServers) {
        ports[itServers->getPort()].addVirtualHost(*itServers);
    }
}