#ifndef SERVERIN_HPP
#define SERVERIN_HPP

#include "SocketException.hpp"
#include <string.h> //memset
#include <fcntl.h> //fcntl
#include <netinet/in.h> //sockaddr_in
#include <stdlib.h> //exit

#define PORT 8080

class   ServerIn {
    private:
    int listen_sock_fd;
    struct sockaddr_in addr;
    int epoll_fd; // Make a class for that

    public:
    ServerIn();
    ~ServerIn();
    ServerIn(const ServerIn& src);
    ServerIn& operator=(const ServerIn& src);
    void    createSocket();
    void    bindSocket();
    void    makeSocketNonBlocking();
    void    addToEpoll(int epoll_fd, int listen_sock_fd, int epollin); // Make a class for that SUPP EPOLLIN IL EST ACCESSIBLE DE PARTO"UT
    void    startToListen();
};

#endif