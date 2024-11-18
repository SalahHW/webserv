#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>
#include <map>

#include "Block.hpp"
#include "Client.hpp"
#include "ConfigExtractor.hpp"
#include "ConfigFile.hpp"
#include "EpollException.hpp"
#include "Location.hpp"

#define MAX_EVENTS 100000

class Server;
class Client;

class ServerHandler {
 public:
  ~ServerHandler();
  ServerHandler(const ConfigFile& configFile);
  ServerHandler(const ServerHandler& src);
  ServerHandler& operator=(const ServerHandler& src);
  void addToEpoll(int fdToAdd) const;

  void displayServerHandlerInfo() const;

 private:
  ServerHandler();

  void epollInit();
  void serversStart();
  void startToListen();
  void handleNewConnection(Server& server);
  void handleClientRead(int clientFd);
  void handleClientWrite(int clientFd);
  void closeClientConnection(int clientFd);
  Client* findClientByFd(int clientFd);
  void modifyEpollEvent(int fd, uint32_t events);

  std::map<int, Server> serversList;
  int epollFd;
  int nbEvents;
};