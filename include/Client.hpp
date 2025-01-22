#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include <deque>
#include <iostream>
#include <string>

#include "Port.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "color_macros.hpp"

class Port;

enum ClientStatus {
  WAITING,
  PROCESSING,
  READY,
};

class Client {
 public:
  ~Client();
  Client(int listenFd, int connectionFd, Port* port);

  void initEv();
  std::string& getBuffer();
  int getListenFd() const;
  int getConnectionFd() const;

  void closeConnection();
  void appendToBuffer(const char* data, size_t len);
  void clearBuffer();
  int readFromClient();

  void clientRoutine();
  void requestRoutine();
  void responsesRoutine();
  void eventToOut();
  void eventToIn();

  ClientStatus status;

 private:
  struct epoll_event ev;
  int listenFd;
  int connectionFd;
  Port* associatedPort;
  std::string buffer;
  std::deque<Response> responses;
  std::deque<Request> requests;

  Client();
  Client(const Client& other);
  Client& operator=(const Client& other);
};
