#pragma once

#include <map>
#include <set>
#include <string>

#include "Location.hpp"
#include "Server.hpp"

class ConfigFinalizer {
 public:
  ~ConfigFinalizer();
  ConfigFinalizer();
  ConfigFinalizer(const ConfigFinalizer &other);
  ConfigFinalizer &operator=(const ConfigFinalizer &other);

  void finalizeConfig(std::map<int, Server> &serversList);
  void finalizeServerConfig(Server &server);
  void finalizeLocationConfig(Location &location, const Server &parentServer);

  bool good() const;

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

  // Default server mapping (key: port, value: server pointer)
  std::map<int, Server *> defaultServers;

  void assignDefaultServers(std::map<int, Server> &serversList);
};