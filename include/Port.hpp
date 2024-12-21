#pragma once

#include "Server.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class Port {
public:
    ~Port();
    Port();
    Port(const Port& src);
    Port& operator=(const Port& src);

    void setupSocket();
    void bindSocket();
    void makeSocketNonBlocking();
    void startListening();

    void addVirtualHost(const Server& server);

    bool good() const;
    int getPort() const;
    int getListenFd() const;

    void setPort(int port);
    void setListenFd(int fd);

private:
    bool isValid;
    int port;
    int listenFd;
    std::map<std::string, Server> virtualHosts;
    struct sockaddr_in addr;
};

