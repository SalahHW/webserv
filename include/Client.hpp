#pragma once

#include <unistd.h>

#include <iostream>
#include <queue>
#include <string>

#include "Port.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "color_macros.hpp"

class Port;

class Client {
 public:
  ~Client();
  Client(int listenFd, int connectionFd, Port* port);

  std::string& getBuffer();
  int getListenFd() const;
  int getConnectionFd() const;

  void closeConnection();
  void appendToBuffer(const char* data, size_t len);
  void clearBuffer();
  void createResponse();

 private:
  int listenFd;
  int connectionFd;
  Port* associatedPort;
  std::string buffer;
  std::queue<Request*> requests;
  std::queue<Response*> responses;

  Client();
  Client(const Client& other);
  Client& operator=(const Client& other);
};
