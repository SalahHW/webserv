#pragma once

// #include <map>
#include <string>
#include <unistd.h>

// #include "HttpStatusCodeDeterminer.hpp"
// #include "ParseRequest.hpp"
// #include "ResponseHandler.hpp"
// #include "Server.hpp"

class Client {
public:
    ~Client();
    Client(int fd);

    void setDestinationFd(int destinationFd);

    std::string& getBuffer();
    int getDestinationFd() const;

    void closeConnection();
    void appendToBuffer(const char* data, size_t len);
    void clearBuffer();

private:
    int fd;
    int destinationFd;
    std::string buffer;

    Client();
    Client(const Client& other);
    Client& operator=(const Client& other);
    //  public:
    //   Client(int client_fd, const Server& server);
    //   ~Client();
    //   // Client& operator=(const Client& src);
    //   Client(const Client& other);

    //   const int& getClientFd() const;
    //   bool shouldCloseConnection() const;
    //   void setConnectionShouldClose(bool shouldClose);

    //   void appendToRequestBuffer(const std::string& data);
    //   bool hasDataToWrite() const;
    //   ssize_t sendData();

    //  private:
    //   int client_fd;
    //   RequestParsed request;
    //   std::string requestBuffer;
    //   std::string responseBuffer;
    //   bool connectionShouldClose;
    //   size_t bytesSent;
    //   const Server& server;

    //   void processRequest();
    //   void setResponse(const std::string& response);
    //   void closeClientSocket();
    //   bool isRequestComplete() const;
    //   void parseRequest();
    //   void handleResponse();
    //   void prepareForSending();
    //   void checkConnectionPersistence();
    //   void handleError(const std::string& functionName);
};
