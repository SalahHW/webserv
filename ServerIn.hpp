#ifndef SERVERIN_HPP
#define SERVERIN_HPP

#include "SocketException.hpp"
#include <string.h> //memset
#include <fcntl.h> //fcntl
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h> //exit
#include <iostream> //cerr
#include <unistd.h>

#define PORT 8080

class   ServerIn {
    private:
    int listen_sock_fd;
    struct sockaddr_in addr;

    public:
    ServerIn();
    ~ServerIn();
    ServerIn(const ServerIn& src);
    ServerIn& operator=(const ServerIn& src);
    void    createSocket();
    void    bindSocket();
    void    makeSocketNonBlocking();
    void    setToListen();
    int     getListenSockFd();
    struct sockaddr_in  getAddr();
};

#endif