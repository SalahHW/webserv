#ifndef RESPONSESENDER_HPP
#define RESPONSESENDER_HPP

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <map>
#include <string>

#include "Client.hpp"
#include "Response.hpp"

class ResponseSender {
 public:
  ResponseSender();
  ~ResponseSender();

  void addResponse(Client& client, const Response& response);
  void sendPendingResponses();
  bool isSendComplete(int clientFd) const;

 private:
  struct ResponseState {
    std::string dataToSend;
    size_t bytesSent;
  };

  std::map<int, ResponseState> pendingResponses;

  std::string buildRawResponse(const Response& response) const;
  void sendChunk(int clientFd, ResponseState& responseState);
};

#endif
