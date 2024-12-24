#pragma once

#include "EventReporter.hpp"
#include "Port.hpp"
#include <iostream>
#include <map>

class ServerManager {
public:
    ~ServerManager();
    ServerManager(std::map<int, Port*> ports);
    ServerManager(const ServerManager& other);
    ServerManager& operator=(const ServerManager& other);

    bool good() const;
    void start();
    void handleEvent(int fd, uint32_t events);

private:
    ServerManager();

    bool isValid;
    std::map<int, Port*> ports;
    EventReporter eventReporter;
    bool isRunning;

    bool initializePorts();
    void makePortsListening();
    void addPortsToEventReporter();
};