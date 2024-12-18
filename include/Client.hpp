#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <ctime>
#include <deque>
#include <map>
#include <string>

#include "Request.hpp"
#include "Response.hpp"

class Client {
 public:
  Client();
  Client(int fd);
  ~Client();

  // Getters
  int getClientFd() const;
  bool shouldCloseConnection() const;
  time_t getLastActivity() const;
  const std::deque<Response>& getResponseQueue() const;

  // Setters
  void setConnectionShouldClose(bool shouldClose);
  void setLastActivity(time_t activityTime);
  void enqueueResponse(const Response& response);

 private:
  int client_fd;
  bool connectionShouldClose;
  time_t lastActivity;
  std::deque<Response> responseQueue;
};

#endif  // CLIENT_HPP
