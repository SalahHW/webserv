#pragma once

#include <sys/epoll.h>
#include <sys/time.h>
#include <unistd.h>

#include <deque>
#include <iostream>
#include <string>

#include "Port.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "color_macros.hpp"

class Port;

class Client
{
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

  void requestRoutine();
  void responsesRoutine();
  void eventToOut();
  void eventToIn();
  void eventToErr();
  double getCurrentTime(void);
  void treatAPost();
  size_t parseContentLength(const std::string& headers);
  std::string removeFinalBoundary(const std::string& input);
  void handleGet();
  void handlePost();
  void handleDelete();
  void handleCgi();
  int getEvent() const;

  int epollFd;
  double lastActivity;

  private:
  struct epoll_event ev;
  int listenFd;
  int connectionFd;
  Port* associatedPort;
  std::string buffer;
  Request* request;

  Client();
  Client(const Client& other);
  Client& operator=(const Client& other);
};
