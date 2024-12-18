#include "Client.hpp"

#include <unistd.h>

Client::Client()
    : client_fd(-1), connectionShouldClose(false), lastActivity(time(NULL)) {}

Client::Client(int fd)
    : client_fd(fd), connectionShouldClose(false), lastActivity(time(NULL)) {}

Client::~Client() {}

int Client::getClientFd() const { return client_fd; }
bool Client::shouldCloseConnection() const { return connectionShouldClose; }
time_t Client::getLastActivity() const { return lastActivity; }

const std::deque<Response>& Client::getResponseQueue() const {
  return responseQueue;
}

void Client::setConnectionShouldClose(bool shouldClose) {
  connectionShouldClose = shouldClose;
}

void Client::setLastActivity(time_t activityTime) {
  lastActivity = activityTime;
}

void Client::enqueueResponse(const Response& response) {
  responseQueue.push_back(response);
}
