#pragma once

#include <string>

#include "HttpStatusCodeDeterminer.hpp"
#include "ParseRequest.hpp"
#include "Server.hpp"

class Client {
 public:
  Client(int client_fd, const Server& server);
  ~Client();

  int getClientFd() const;
  bool shouldCloseConnection() const;
  void setConnectionShouldClose(bool shouldClose);

  void appendToRequestBuffer(const std::string& data);
  bool hasDataToWrite() const;
  ssize_t sendData();

 private:
  Client(const Client& other);
  Client& operator=(const Client& other);

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
  void determineStatusCode();
};
