#include "ConfigFinalizer.hpp"

ConfigFinalizer::~ConfigFinalizer() {}

ConfigFinalizer::ConfigFinalizer()
    : defaultServerName("_"),
      defaultServerRoot("/www/"),
      defaultServerIndex("index.html"),
      defaultServerPort(80),
      defaultServerClientMaxBodySize(1000000),
      defaultLocationPath("/"),
      defaultLocationRoot("/www/") {}

ConfigFinalizer::ConfigFinalizer(const ConfigFinalizer &other) {
  *this = other;
}

ConfigFinalizer &ConfigFinalizer::operator=(const ConfigFinalizer &other) {
  if (this != &other) {
    this->defaultServerName = other.defaultServerName;
    this->defaultServerRoot = other.defaultServerRoot;
    this->defaultServerIndex = other.defaultServerIndex;
    this->defaultServerPort = other.defaultServerPort;
    this->defaultServerClientMaxBodySize = other.defaultServerClientMaxBodySize;
    this->defaultLocationPath = other.defaultLocationPath;
    this->defaultLocationRoot = other.defaultLocationRoot;
    this->defaultServers = other.defaultServers;
  }
  return *this;
}

void ConfigFinalizer::finalizeConfig(std::map<int, Server> &serversList) {
  assignDefaultServers(serversList);

  for (std::map<int, Server>::iterator serverIt = serversList.begin();
       serverIt != serversList.end(); ++serverIt) {
    finalizeServerConfig(serverIt->second);
    std::map<std::string, Location> &locations =
        serverIt->second.getLocations();

    for (std::map<std::string, Location>::iterator locationIt =
             locations.begin();
         locationIt != locations.end(); ++locationIt) {
      finalizeLocationConfig(locationIt->second, serverIt->second);
    }
  }
}

void ConfigFinalizer::finalizeServerConfig(Server &server) {
  if (!server.isNameDefined()) {
    server.setName(defaultServerName);
  }
  if (!server.isPortDefined()) {
    server.setPort(defaultServerPort);
  }
  if (!server.isClientMaxBodySizeDefined()) {
    server.setClientMaxBodySize(defaultServerClientMaxBodySize);
  }
}

void ConfigFinalizer::finalizeLocationConfig(Location &location,
                                             const Server &parentServer) {
  (void)parentServer;
  if (!location.isPathDefined()) {
    location.setPath(defaultLocationPath);
  }
  if (!location.isRootDirectoryDefined()) {
    location.setRootDirectory(defaultLocationRoot);
  }
}

bool ConfigFinalizer::good() const { return isValid; }

void ConfigFinalizer::assignDefaultServers(std::map<int, Server>& serversList) {
  for (std::map<int, Server>::iterator it = serversList.begin(); it != serversList.end(); ++it) {
    Server& server = it->second;
    int port = server.getPort();

    if (server.isExplicitlyDefault()) {
      processExplicitDefault(server, port);
    } else {
      assignImplicitDefault(server, port);
    }
  }
}

void ConfigFinalizer::processExplicitDefault(Server& server, int port) {
  if (defaultServers.find(port) != defaultServers.end()) {
    std::cerr << "Error: Multiple servers defined as default for port " << port << "." << std::endl;
    isValid = false;
  } else {
    defaultServers[port] = &server;
  }
}

void ConfigFinalizer::assignImplicitDefault(Server& server, int port) {
  if (defaultServers.find(port) == defaultServers.end()) {
    defaultServers[port] = &server;
  }
}