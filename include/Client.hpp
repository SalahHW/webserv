#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <string>

#include "ParseRequest.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ResponseHandler.hpp"
#include "Server.hpp"

class Client {
 public:
  Client(int client_fd, const Server& server);
  ~Client();

  int getClientFd() const;
  size_t getBytesSent() const;
  bool shouldCloseConnection() const;
  void setConnectionShouldClose(bool shouldClose);

  void appendToRequestBuffer(const std::string& data);
  void processRequests();
  std::deque<Response> getResponseQueue() const;
  void sendResponses();
  bool hasPendingRequests() const;
  void enqueueResponse(const Response& response);

  std::map<int, std::pair<Request, Response> > getResponseMap() const;

 private:
  void closeClientSocket();

  size_t bytesSent;
  int client_fd;
  bool connectionShouldClose;
  int currentRequestId;
  std::map<int, std::pair<Request, Response> > pendingRequests;
  std::deque<Response> responseQueue;
  ResponseHandler* responseHandler;
  std::string requestBuffer;
};

#endif  // CLIENT_HPP
