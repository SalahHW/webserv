#pragma once

#include <fcntl.h>       // Pour fcntl
#include <netdb.h>       // Pour getaddrinfo, freeaddrinfo, gai_strerror
#include <netinet/in.h>  // Pour sockaddr_in, INADDR_ANY
#include <stdlib.h>      // Pour exit
#include <string.h>      // Pour memset, memcpy
#include <sys/socket.h>  // Pour socket, bind, listen, accept
#include <unistd.h>      // Pour close

#include <cstdio>    // pour sprintf
#include <iostream>  // Pour std::cerr
#include <map>
#include <vector>

#include "SocketException.hpp"

class Location;
class Client;

class Server {
 public:
  ~Server();
  Server();
  Server(const Server& src);
  Server& operator=(const Server& src);

  void setListenFd();
  void setPort(int port);
  void setClientMaxBodySize(int size);
  void setName(const std::string& name);
  void setErrorPages(std::map<int, std::string> errorPages);
  void setDefault(bool isDefault);
  void setExplicitlyDefault(bool isExplicitlyDefault);

  void addLocation(const Location& location);
  void addErrorPage(int errorCode, std::string errorPath);

  int getListenFd() const;
  int getPort() const;
  int getClientMaxBodySize() const;
  const std::string& getName() const;
  const std::map<int, std::string>& getErrorPages() const;
  bool isDefaultServer() const;
  bool isExplicitlyDefault() const;

  bool isListenFdDefined() const { return hasListenFd; }
  bool isPortDefined() const { return hasPort; }
  bool isNameDefined() const { return hasName; }
  bool isClientMaxBodySizeDefined() const { return hasClientMaxBodySize; }
  bool isErrorPagesDefined() const { return hasErrorPages; }
  bool isLocationsDefined() const { return hasLocations; }

  void displayServerInfo() const;

  void paramFd();
  void addClientToServer(Client* clientToAdd);

  std::map<int, Client*>& getClientsList();
  std::map<std::string, Location>& getLocations();
  const std::map<std::string, Location>& getLocations() const;

 private:
  int listenFd;
  int port;
  std::string name;
  int clientMaxBodySize;
  bool isDefault;
  bool explicitlyDefault;
  std::map<int, std::string> errorPages;
  std::map<std::string, Location> locations;
  std::map<int, Client*> clientsList;
  struct sockaddr_in addr;

  bool hasListenFd;
  bool hasPort;
  bool hasName;
  bool hasClientMaxBodySize;
  bool hasErrorPages;
  bool hasLocations;

  void resolveHostName();
  void bindSocket() const;
  void setToListen() const;
  void makeSocketNonBlocking() const;
};