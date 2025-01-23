#pragma once

#include <sys/epoll.h>
#include <unistd.h>

#include <cerrno>
#include <iostream>

class ServerManager;

class EventReporter {
 public:
  ~EventReporter();
  EventReporter();

  bool good() const;

  bool addFD(int fd);
  int getNextEvent(uint32_t& eventFlag);
  int getEpollFd() const { return epollFd; }

 private:
  bool isValid;
  int epollFd;

  void initializeEpoll();
  void closeEpoll();

  EventReporter(const EventReporter& other);
  EventReporter& operator=(const EventReporter& other);
};