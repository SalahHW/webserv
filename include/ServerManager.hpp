#pragma once

#include "Client.hpp"
#include "EventReporter.hpp"
#include "Port.hpp"
#include <iostream>
#include <map>
#include <set>

class ServerManager {
public:
    ~ServerManager();
    ServerManager(std::map<int, Port*> ports);
    ServerManager(const ServerManager& other);
    ServerManager& operator=(const ServerManager& other);

    bool good() const;
    void start();
    void runRoutine();
    void handleEvent(int fd, uint32_t events);
    bool isListeningSocket(int fd) const;

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