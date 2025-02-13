#include "Sender.hpp"

#include <fcntl.h>

Sender::Sender(Response& response, int sockfd, Request& request, Client* client)
{
  sendOnFd(response, sockfd, request, client);
}

Sender::~Sender() { }

void Sender::sendOnFd(Response& response, int sockfd, Request& request, Client* client)
{
  std::cout << response.getFullHeader().data() << std::endl;
  std::cout << response.getBody().data() << std::endl;
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
  } // else if (ret < 0) {
  //   perror("send error");
  //   std::cerr << "Errno: " << errno << std::endl;

  //   if (errno == EPIPE) {
  //     std::cerr << "Connection closed by peer (EPIPE)." << std::endl;
  //   } else if (errno == ECONNRESET) {
  //     std::cerr << "Connection reset by peer  (ECONNRESET)." << std::endl;
  //   } else if (errno == ENOTCONN) {
  //     std::cerr << "Socket is not connected (ENOTCONN)." << std::endl;
  //   }
  // }
  if (response.getContentLength().empty() && !request.getIsTreated())
  {
    ret = send(sockfd, "\r\n", 2, MSG_NOSIGNAL);
  }

  // std::cout << "What is sended: " << hexStr;
  // std::cout.write(Response.data(), responseSize) << "\r\n" << std::endl;
  // std::cout << "test" << std::endl;
}