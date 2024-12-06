#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>
#include <map>

#include "Client.hpp"
#include "ConfigExtractor.hpp"
#include "ConfigFile.hpp"
#include "Location.hpp"

#define MAX_EVENTS 100000

class Server;
class Client;

class ServerHandler {
 public:
  // Destructor
  ~ServerHandler();

  // Constructors
  ServerHandler(const ConfigFile& configFile);
  ServerHandler(const ServerHandler& src);
  ServerHandler& operator=(const ServerHandler& src);

  // Methods
  void addToEpoll(int fdToAdd) const;
  void displayServerHandlerInfo() const;

 private:
  // Private default constructor to prevent instantiation without config
  ServerHandler();

  // Initialization methods
  void initializeEpoll();
  void initializeServers();
  void startListening();

  // Event handling methods
  void handleNewConnection(Server& server);
  void handleClientRead(int clientFd);
  void handleClientWrite(int clientFd);
  void closeClientConnection(int clientFd);
  Client* findClientByFd(int clientFd);
  void modifyEpollEvent(int fd, uint32_t events);

  // Member variables
  std::map<int, Server> serversList;
  int epollFd;
  int nbEvents;
};
