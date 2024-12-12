// ServerHandler.hpp

#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <map>

#include "ConfigExtractor.hpp"
#include "ConfigFile.hpp"
#include "Server.hpp"

#define MAX_EVENTS 1024
#define CLIENT_TIMEOUT 45

class Client;

class ServerHandler {
 public:
  ServerHandler(const ConfigFile& configFile);
  ~ServerHandler();

  void startListening();
  void displayServerHandlerInfo() const;

  void removeClient(int fd);

 private:
  void initializeEpoll();
  void initializeServers();
  void addToEpoll(int fdToAdd) const;
  void handleNewConnection(Server& server);
  void handleClientRead(Client* clientFd);
  void handleClientWrite(Client* clientFd);
  Client* findClientByFd(int clientFd);
  void modifyEpollEvent(Client* client, uint32_t events);
  void addClientToEpoll(Client* client, Server& server);

  void checkClientTimeouts();

  int epollFd;
  int nbEvents;
  std::map<int, Server> serversList;
};

#endif  // SERVERHANDLER_HPP
