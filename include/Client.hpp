#pragma once

#include "color_macros.hpp"
#include "Port.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

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

private:
    int listenFd;
    int connectionFd;
    Port* associatedPort;
    std::string buffer;

    Client();
    Client(const Client& other);
    Client& operator=(const Client& other);
};
