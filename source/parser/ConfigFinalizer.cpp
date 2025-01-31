#include "ConfigFinalizer.hpp"

ConfigFinalizer::~ConfigFinalizer() { }

ConfigFinalizer::ConfigFinalizer()
    : isValid(true)
    , defaultServerName("_")
    , defaultServerRoot("/www/")
    , defaultServerIndex("index.html")
    , defaultServerClientBodyTempPath("/client_temp/")
    , defaultServerPort(80)
    , defaultServerClientMaxBodySize(1 * 1024 * 1024)
    , defaultServerClientTimeOut(60)
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
  if (this != &other)
  {
    this->isValid = other.isValid;
    this->defaultServerName = other.defaultServerName;
    this->defaultServerRoot = other.defaultServerRoot;
    this->defaultServerIndex = other.defaultServerIndex;
    this->defaultServerClientBodyTempPath = other.defaultServerClientBodyTempPath;
    this->defaultServerPort = other.defaultServerPort;
    this->defaultServerClientMaxBodySize = other.defaultServerClientMaxBodySize;
    this->defaultServerClientTimeOut = other.defaultServerClientTimeOut;
    this->defaultLocationPath = other.defaultLocationPath;
    this->defaultLocationRoot = other.defaultLocationRoot;
    this->ports = other.ports;
  }
  return *this;
}

bool ConfigFinalizer::good() const { return this->isValid; }

const std::map<int, Port*>& ConfigFinalizer::getPorts() const { return ports; }

void ConfigFinalizer::finalizeConfig(std::vector<Server>& servers)
{
  std::vector<Server>::iterator itServer;
  for (itServer = servers.begin(); itServer != servers.end(); ++itServer)
  {
    finalizeServer(*itServer);
    finalizeLocations(*itServer);
    addUsedPort(itServer->getPort());
  }
  createPorts();
  assignServersToPorts(servers);
}

void ConfigFinalizer::finalizeServer(Server& server)
{
  if (!server.isPortDefined())
  {
    server.setPort(defaultServerPort);
  }
  if (!server.isNameDefined())
  {
    server.setName(defaultServerName);
  }
  if (!server.isClientBodyTempPathDefined())
  {
    server.setClientBodyTempPath(defaultServerClientBodyTempPath);
  }
  if (!server.isClientMaxBodySizeDefined())
  {
    server.setClientMaxBodySize(defaultServerClientMaxBodySize);
  }
  if (!server.isClientTimeOutDefined())
  {
    server.setClientTimeOut(defaultServerClientTimeOut);
  }
  // TODO: Complete default Server configuration
}

void ConfigFinalizer::finalizeLocations(Server& server)
{
  std::map<std::string, Location>& locations = server.getLocations();
  std::map<std::string, Location>::iterator itLocation;

  for (itLocation = locations.begin(); itLocation != locations.end(); ++itLocation)
  {
    Location& location = itLocation->second;

    if (!location.isPathDefined())
    {
      location.setPath(defaultLocationPath);
    }
    if (!location.isRootDirectoryDefined())
    {
      location.setRootDirectory(defaultLocationRoot);
    }
    if (!location.isIndexFileDefined())
    {
      location.setIndexFile(defaultServerIndex);
    }
    if (!location.isClientMaxBodySizeDefined())
    {
      location.setClientMaxBodySize(server.getClientMaxBodySize());
    }
    if (!location.isClientBodyTempPathDefined())
    {
      location.setClientBodyTempPath(server.getClientBodyTempPath());
    }
    if (!location.isClientTimeOutDefined())
    {
      location.setClientTimeOut(server.getClientTimeOut());
    }
    // TODO: Complete default Location configuration
  }
}

void ConfigFinalizer::addUsedPort(int port)
{
  if (std::find(usedPorts.begin(), usedPorts.end(), port) == usedPorts.end())
  {
    usedPorts.push_back(port);
  }
}

void ConfigFinalizer::createPorts()
{
  std::list<int>::const_iterator itUsedPorts;

  for (itUsedPorts = usedPorts.begin(); itUsedPorts != usedPorts.end(); ++itUsedPorts)
  {
    Port* port = new Port();
    port->setPort(*itUsedPorts);
    ports[*itUsedPorts] = port;
  }
}

void ConfigFinalizer::assignServersToPorts(const std::vector<Server>& servers)
{
  std::vector<Server>::const_iterator itServers;
  bool defaultVirtualHost = false;

  for (itServers = servers.begin(); itServers != servers.end(); ++itServers)
  {
    // ports[itServers->getPort()]->addVirtualHost(*itServers);
    Port* port = ports[itServers->getPort()];
    port->addVirtualHost(*itServers);

    if (!port->getHasDefaultVirtualHost())
    {
      port->setDefaultVirtualHostName(itServers->getName());
      port->setHasDefaultVirtualHostName(true);
    }

    if (itServers->isExplicitlyDefault())
    {
      if (defaultVirtualHost)
      {
        std::cerr << "Error: Multiple default servers defined for port "
                  << itServers->getPort() << std::endl;
        isValid = false;
        return;
      }
      port->setDefaultVirtualHostName(itServers->getName());
      port->setHasDefaultVirtualHostName(true);
      defaultVirtualHost = true;
    }
  }
}