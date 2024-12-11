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

#include "Client.hpp"
#include "ConfigExtractor.hpp"
#include "ConfigFile.hpp"
#include "Server.hpp"

#define MAX_EVENTS 1024

class ServerHandler {
 public:
  ServerHandler(const ConfigFile& configFile);
  ~ServerHandler();

  void startListening();
  void displayServerHandlerInfo() const;

 private:
  void initializeEpoll();
  void initializeServers();
  void addToEpoll(int fdToAdd) const;
  void handleNewConnection(Server& server);
  void handleClientRead(int clientFd);
  void handleClientWrite(int clientFd);
  void closeClientConnection(int clientFd);
  Client* findClientByFd(int clientFd);
  void modifyEpollEvent(int fd, uint32_t events);

  int epollFd;
  int nbEvents;
  std::map<int, Server> serversList;
};

#endif  // SERVERHANDLER_HPP
