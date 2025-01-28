#pragma once

#include <iostream> // Pour std::cerr
#include <map>
#include <vector>

class Location;
class Client;

class Server
{
  public:
  ~Server();
  Server();
  Server(const Server& src);
  Server& operator=(const Server& src);

  void setPort(int port);
  void setClientMaxBodySize(int size);
  void setName(const std::string& name);
  void setClientBodyTempPath(const std::string& clientBodyTempPath);
  void setErrorPages(std::map<size_t, std::string> errorPages);
  void setExplicitlyDefault(bool isExplicitlyDefault);

  void addLocation(const Location& location);
  void addErrorPage(int errorCode, std::string errorPath);

  int getPort() const;
  int getClientMaxBodySize() const;
  const std::string& getName() const;
  const std::string& getClientBodyTempPath() const;
  const std::map<size_t, std::string>& getErrorPages() const;
  bool isExplicitlyDefault() const;

  bool isPortDefined() const { return hasPort; };
  bool isNameDefined() const { return hasName; };
  bool isClientBodyTempPathDefined() const { return hasClientBodyTempPath; };
  bool isClientMaxBodySizeDefined() const { return hasClientMaxBodySize; };
  bool isErrorPagesDefined() const { return hasErrorPages; };
  bool isLocationsDefined() const { return hasLocations; };

  void displayServerInfo() const;

  std::map<std::string, Location>& getLocations();
  const std::map<std::string, Location>& getLocations() const;

  private:
  int port;
  std::string name;
  std::string clientBodyTempPath;
  int clientMaxBodySize;
  bool isDefault;
  bool explicitlyDefault;
  bool hasPort;
  bool hasName;
  bool hasClientBodyTempPath;
  bool hasClientMaxBodySize;
  bool hasErrorPages;
  bool hasLocations;

  std::map<size_t, std::string> errorPages;
  std::map<std::string, Location> locations;
};