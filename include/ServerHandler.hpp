#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <map>
#include <string>

#include "ClientManager.hpp"
#include "Server.hpp"

#ifndef MAX_EVENTS
#define MAX_EVENTS 64
#endif

class ServerHandler {
 public:
  ServerHandler(std::map<int, Server> inputServers);
  ~ServerHandler();

  void startListening();

 private:
  int epollFd;
  std::map<int, Server> servers;
  ClientManager clientManager;

  void initializeEpoll();
  void addServerToEpoll(int fd);
  void handleNewConnection(int listenFd);
};

#endif
