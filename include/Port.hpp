#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>

#include "Client.hpp"
#include "Server.hpp"
#include "VirtualHost.hpp"

class Port {
 public:
  ~Port();
  Port();

  void initialize();
  void startListening();
  void processClientData(Client& client);

  void addVirtualHost(const Server& server);

  bool good() const;
  void displayHosts() const;

  int getPort() const;
  int getListenFd() const;
  const std::map<std::string, VirtualHost>& getVirtualHosts() const;

  void setPort(int port);
  void setListenFd(int fd);

 private:
  bool isValid;
  int port;
  int listenFd;
  std::map<std::string, VirtualHost> virtualHosts;
  struct sockaddr_in addr;

  void setupSocket();
  void bindSocket();
  void makeSocketNonBlocking();
  VirtualHost createVirtualHost(const Server& server);

  Port(const Port& src);
  Port& operator=(const Port& src);
};
