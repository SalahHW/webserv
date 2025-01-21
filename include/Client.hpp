#pragma once

#include "Port.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "color_macros.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

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

    std::string& getBuffer();
    int getListenFd() const;
    int getConnectionFd() const;

    void closeConnection();
    void appendToBuffer(const char* data, size_t len);
    void clearBuffer();
    int readFromClient();

    void executeNextTask();

    ClientStatus status;
private:
    int listenFd;
    int connectionFd;
    Port* associatedPort;
    std::string buffer;

    Client();
    Client(const Client& other);
    Client& operator=(const Client& other);
};
