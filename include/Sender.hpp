#pragma once

#include <sys/socket.h>

#include <cstddef>
#include <string>

#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Sender
{
  public:
  Sender(Response& response, int sockfd, Request& request, Client* client);

  ~Sender();
  void sendOnFd(Response& response, int sockfd, Request& request, Client* client);

  private:
  Sender(const Sender& other);
  Sender& operator=(const Sender& other);
};