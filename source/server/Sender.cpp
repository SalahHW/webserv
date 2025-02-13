#include "Sender.hpp"

#include <fcntl.h>

Sender::Sender(Response& response, int sockfd, Request& request, Client* client)
{
  sendOnFd(response, sockfd, request, client);
}

Sender::~Sender() { }

void Sender::sendOnFd(Response& response, int sockfd, Request& request, Client* client)
{
  if (!response.getFullHeader().empty())
  {
    send(sockfd, response.getFullHeader().data(),
        response.getFullHeader().size(), MSG_NOSIGNAL);
  }

  std::vector<char> Response = response.getBody();
  size_t responseSize = Response.size();
  int ret = 0;
  std::stringstream hex;
  std::string hexStr;

  if (Response.empty())
  {
    std::cerr << "Response is empty, nothing to send." << std::endl;
    client->eventToErr();
    return;
  }
  if (!request.getIsTreated() && response.getContentLength().empty())
  {
    hex << std::hex << responseSize;
    hexStr = hex.str() + "\r\n";

    ret = send(sockfd, hexStr.c_str(), hexStr.size(), MSG_NOSIGNAL);
  }
  ret = send(sockfd, Response.data(), responseSize, MSG_NOSIGNAL);

  if (ret > 0)
  {
    response.setBytesSent(response.getBytesSent() + ret);
  }
  if (response.getContentLength().empty() && !request.getIsTreated())
  {
    ret = send(sockfd, "\r\n", 2, MSG_NOSIGNAL);
  }
}