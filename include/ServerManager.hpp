#pragma once

#include <csignal>
#include <iostream>
#include <map>
#include <set>

#include "Client.hpp"
#include "EventReporter.hpp"
#include "Port.hpp"

#define TIMEOUT 10000

class ServerManager
{
  public:
  ~ServerManager();
  ServerManager(std::map<int, Port*> ports);
  ServerManager(const ServerManager& other);
  ServerManager& operator=(const ServerManager& other);

  bool good() const;
  void start();
  void stop();
  void runRoutine();
  void handleEvent(int fd, uint32_t events);
  void setIsRunning(bool running);
  bool isListeningSocket(int fd) const;

  static ServerManager* instance;

  private:
  ServerManager();

  bool isValid;
  std::map<int, Port*> ports;
  std::map<int, Client*> clients;
  std::set<int> listeningSockets;
  EventReporter eventReporter;
  bool isRunning;

  bool initializePorts();
  void addPortsToEventReporter();
  void acceptConnection(int listenFd);
  void handleEpollIn(int listenFd);
  void handleEpollOut(int listenFd);
  void handleEpollErr(int listenFd);
  void closeConnection(int clientFd);
  void readFromClient(int clientFd);
};