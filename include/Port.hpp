#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>

#include "Client.hpp"
#include "Server.hpp"
#include "VirtualHost.hpp"

class Port {
public:
    ~Port();
    Port();

    void initialize();
    void startListening();
    void processClientData(Client& client);

    void addVirtualHost(const Server& server);

    bool good() const;
    void displayHosts() const;

    int getPort() const;
    int getListenFd() const;
    const std::map<std::string, VirtualHost>& getVirtualHosts() const;
    bool getHasVirtualHost() const;
    bool getHasDefaultVirtualHost() const;
    const std::string& getDefaultVirtualHostName() const;

    void setPort(int port);
    void setListenFd(int fd);
    void setDefaultVirtualHostName(const std::string& hostName);
    void setHasDefaultVirtualHostName(bool value);

private:
    bool isValid;
    int port;
    int listenFd;
    bool hasVirtualHost;
    bool hasDefaultVirtualHost;
    std::map<std::string, VirtualHost> virtualHosts;
    std::string defaultVirtualHostName;
    struct sockaddr_in addr;

    void setupSocket();
    void bindSocket();
    void makeSocketNonBlocking();
    VirtualHost createVirtualHost(const Server& server);

    Port(const Port& src);
    Port& operator=(const Port& src);
};
