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

class ConfigFinalizer
{
  public:
  ~ConfigFinalizer();
  ConfigFinalizer();
  ConfigFinalizer(const ConfigFinalizer& other);
  ConfigFinalizer& operator=(const ConfigFinalizer& other);

  bool good() const;
  const std::map<int, Port*>& getPorts() const;

  void finalizeConfig(std::vector<Server>& servers);
  void finalizeServer(Server& server);
  void finalizeLocations(Server& server);
  void addUsedPort(int port);
  void createPorts();
  void assignServersToPorts(const std::vector<Server>& servers);

  private:
  // Flags
  bool isValid;

  // Default Server Values
  std::string defaultServerName;
  std::string defaultServerRoot;
  std::string defaultServerIndex;
  std::string defaultServerClientBodyTempPath;
  int defaultServerPort;
  int defaultServerClientMaxBodySize;
  int defaultServerClientTimeOut;

  // Default Location Values
  std::string defaultLocationPath;
  std::string defaultLocationRoot;

  std::list<int> usedPorts;

  std::map<int, Port*> ports;
};