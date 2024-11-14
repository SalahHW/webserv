#pragma once

#include "ClientException.hpp"
#include "HttpRequest.hpp"
#include "ResponseHandler.hpp"
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class ServerHandler;

class Client {
private:
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int flags;
    RequestParsed request;
    std::string requestBuffer;
    bool connectionShouldClose;

public:
    Client(int listen_sock_fd);
    ~Client();
    Client(const Client& src);
    Client& operator=(const Client& src);
    const int& getClientFd() const;
    const struct sockaddr_in& getClientAddr() const;
    const socklen_t& getClientLen() const;
    const int& getFlags() const;
    void CreateClientSock(int listen_sock_fd);
    void makeSocketNonBlocking();
    void setSocketBufferSize(int recvBufSize, int sendBufSize) const;
    void readRequest();
    void setRequest(std::string request);
    void appendToRequestBuffer(const std::string& data);
    bool shouldCloseConnection() const;
    void setConnectionShouldClose(bool shouldClose);
};