#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include "Server.hpp"
#include <iostream>
#include <map>
#include <string>

class Server;

class ServerHandler {
private:
    std::map<int, Server> serverList;
    ServerHandler(const ServerHandler& src);
    ServerHandler& operator=(const ServerHandler& src);

public:
    ServerHandler();
    ~ServerHandler();
    void addServer(int serverFd, const Server& src);
};

#endif