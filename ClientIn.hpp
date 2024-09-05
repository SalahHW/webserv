#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ClientException.hpp"
#include "EpollManagement.hpp"
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class EpollManagement;

class ClientIn {
private:
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int flags;

public:
    ClientIn(int listen_sock_fd, EpollManagement epoll);
    ~ClientIn();
    ClientIn(const ClientIn& src);
    ClientIn& operator=(const ClientIn& src);
    int getClientFd();
    struct sockaddr_in getClientAddr();
    socklen_t getClientLen();
    int getFlags();
    void CreateClientSock(int listen_sock_fd);
    void makeSocketNonBlocking();
    void setSocketBufferSize(int recvBufSize, int sendBufSize);
};

#endif