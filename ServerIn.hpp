#ifndef SERVERIN_HPP
#define SERVERIN_HPP

#include "SocketException.hpp"
#include <fcntl.h> //fcntl
#include <iostream> //cerr
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h> //exit
#include <string.h> //memset
#include <unistd.h>

#define PORT 8080

class ServerIn {
private:
    int listen_sock_fd;
    struct sockaddr_in addr;

public:
    ServerIn();
    ~ServerIn();
    ServerIn(const ServerIn& src);
    ServerIn& operator=(const ServerIn& src);
    void createSocket();
    void bindSocket() const;
    void makeSocketNonBlocking() const;
    void setToListen() const;
    const int& getListenSockFd() const;
    const struct sockaddr_in& getAddr() const;
};

#endif