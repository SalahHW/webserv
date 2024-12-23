#pragma once

#include "EventReporter.hpp"
#include "Port.hpp"
#include <iostream>
#include <map>

class ServerManager {
public:
    ~ServerManager();
    ServerManager(std::map<int, Port> ports);
    ServerManager(const ServerManager& other);
    ServerManager& operator=(const ServerManager& other);

    void start();
    void handleEvent(int fd, uint32_t events);

private:
    ServerManager();

    std::map<int, Port> ports;
    EventReporter eventReporter;
    bool isRunning;

    void makePortsListening();
    void addPortsToEventReporter();
};