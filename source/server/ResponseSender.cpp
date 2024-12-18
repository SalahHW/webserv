#include "ResponseSender.hpp"

#include <iostream>
#include <sstream>

ResponseSender::ResponseSender() {}
ResponseSender::~ResponseSender() {}

void ResponseSender::addResponse(Client& client, const Response& response) {
  int clientFd = client.getClientFd();
  ResponseState state;
  state.dataToSend = buildRawResponse(response);
  state.bytesSent = 0;
  pendingResponses[clientFd] = state;
}

std::string ResponseSender::buildRawResponse(const Response& response) const {
  std::ostringstream rawResponse;

  rawResponse << "HTTP/1.1 " << response.getStatusCode() << " "
              << response.getReasonPhrase() << "\r\n";

  std::map<std::string, std::string>::const_iterator it =
      response.getHeaders().begin();
  for (; it != response.getHeaders().end(); ++it) {
    rawResponse << it->first << ": " << it->second << "\r\n";
  }

  rawResponse << "\r\n";
  rawResponse << response.getBody();

  return rawResponse.str();
}

void ResponseSender::sendPendingResponses() {
  std::map<int, ResponseState>::iterator it = pendingResponses.begin();
  while (it != pendingResponses.end()) {
    int clientFd = it->first;
    ResponseState& state = it->second;

    sendChunk(clientFd, state);

    if (state.bytesSent >= state.dataToSend.size()) {
      close(clientFd);
      std::map<int, ResponseState>::iterator eraseIt = it;
      ++it;
      pendingResponses.erase(eraseIt);
    } else {
      ++it;
    }
  }
}

void ResponseSender::sendChunk(int clientFd, ResponseState& responseState) {
  size_t remaining = responseState.dataToSend.size() - responseState.bytesSent;
  ssize_t sent =
      send(clientFd, responseState.dataToSend.c_str() + responseState.bytesSent,
           remaining, MSG_DONTWAIT);
  if (sent > 0) {
    responseState.bytesSent += (size_t)sent;
  } else if (sent == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
    std::cerr << "Error sending data to client " << clientFd << ": "
              << std::strerror(errno) << std::endl;
    close(clientFd);
    responseState.bytesSent = responseState.dataToSend.size();
  }
}

bool ResponseSender::isSendComplete(int clientFd) const {
  return (pendingResponses.find(clientFd) == pendingResponses.end());
}
