#ifndef SERVERHANDLER_HPP
#define SERVERHANDLER_HPP

#include "Server.hpp"

class ServerHandler {
private:
    std::vector<Server> serverList;
    ServerHandler(const ServerHandler& src);
    ServerHandler& operator=(const ServerHandler& src);

public:
    ServerHandler();
    ~ServerHandler();
    void addServer(const Server& src);
};

#endif