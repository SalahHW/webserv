#pragma once

#include "HttpStatusCodeDeterminer.hpp"
#include "ParseRequest.hpp"
#include "ResponseHandler.hpp"
#include "Server.hpp"
#include <map>
#include <string>

class Client {
public:
    Client(int client_fd, const Server& server);
    ~Client();
    // Client& operator=(const Client& src);
    Client(const Client& other);

    const int& getClientFd() const;
    bool shouldCloseConnection() const;
    void setConnectionShouldClose(bool shouldClose);

    void appendToRequestBuffer(const std::string& data);
    bool hasDataToWrite() const;
    ssize_t sendData();

private:
    int client_fd;
    RequestParsed request;
    std::string requestBuffer;
    std::string responseBuffer;
    bool connectionShouldClose;
    size_t bytesSent;
    const Server& server;

    void processRequest();
    void setResponse(const std::string& response);
    void closeClientSocket();
    bool isRequestComplete() const;
    void parseRequest();
    void handleResponse();
    void prepareForSending();
    void checkConnectionPersistence();
    void handleError(const std::string& functionName);
};
