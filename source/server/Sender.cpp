#include "Sender.hpp"

#include <fcntl.h>

Sender::Sender(Response& response, int sockfd) { sendOnFd(response, sockfd); }

Sender::~Sender() {}

// void Sender::sendOnFd(Response& response, int sockfd) {
//   const std::string& fullResponse = response.getFullResponse();
//   size_t offset = response.getBytesSent();
//   size_t totalSize = response.getBytesTotal();
//   if (offset >= totalSize) {
//     return;
//   }
//   size_t remain = totalSize - offset;
//   if (remain > 1024) {
//     remain = 1024;
//   }
//   ssize_t ret = send(sockfd, fullResponse.c_str() + offset, remain, 0);
//   if (ret > 0) {
//     response.setBytesSent(offset + ret);
//   } else if (ret < 0) {
//     perror("send");
//   }
// }
void Sender::sendOnFd(Response& response, int sockfd) {
  std::cout << "Starting sendOnFd..." << std::endl;

  const std::string& Response = response.getFullResponse();
  size_t responseSize = Response.size();
  if (Response.empty()) {
    std::cerr << "Response is empty, nothing to send." << std::endl;
    return;
  }
  ssize_t ret = send(sockfd, Response.c_str(), responseSize, MSG_NOSIGNAL);
  if (ret > 0) {
    response.setBytesSent(response.getBytesSent() + ret);
    std::cout << "Successfully sent " << ret << " bytes." << std::endl;
  } else if (ret < 0) {
    perror("send error");
    std::cerr << "Errno: " << errno << std::endl;

    if (errno == EPIPE) {
      std::cerr << "Connection closed by peer (EPIPE)." << std::endl;
    } else if (errno == ECONNRESET) {
      std::cerr << "Connection reset by peer (ECONNRESET)." << std::endl;
    } else if (errno == ENOTCONN) {
      std::cerr << "Socket is not connected (ENOTCONN)." << std::endl;
    }
  }
}
