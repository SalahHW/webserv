#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <ctime>
#include <deque>
#include <iostream>
#include <map>
#include <string>

#include "ParseRequest.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "ResponseHandler.hpp"
#include "Server.hpp"

class ServerHandler;

class Client {
 public:
  Client(int client_fd, const Server& server, ServerHandler* handler);
  ~Client();

  int getClientFd() const;
  std::string getHeader(const std::string& header) const;
  bool shouldCloseConnection() const;
  void setConnectionShouldClose(bool shouldClose);

  void appendToRequestBuffer(const std::string& data);
  void processRequests();
  std::deque<Response> getResponseQueue() const;
  void sendResponses();
  bool hasPendingRequests() const;
  void enqueueResponse(const Response& response);

  std::map<int, std::pair<Request, Response> > getResponseMap() const;

  bool isTimedOut(time_t currentTime, int timeoutDuration) const;

  void updateLastActivity();
  void closeClientSocket();

  void notifyAndDelete();

 private:
  int client_fd;
  bool connectionShouldClose;
  bool pendingClose;
  int currentRequestId;
  std::map<int, std::pair<Request, Response> > pendingRequests;
  std::deque<Response> responseQueue;
  ResponseHandler* responseHandler;
  std::string requestBuffer;

  time_t lastActivity;

  ServerHandler* serverHandler;

  Client(const Client&);
  Client& operator=(const Client&);
};

#endif  // CLIENT_HPP
