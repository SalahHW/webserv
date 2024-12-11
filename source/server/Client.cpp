#include "Client.hpp"

#include "ResponseHandler.hpp"
#include "Server.hpp"

Client::Client(int client_fd, const Server& server)
    : bytesSent(0),
      client_fd(client_fd),
      connectionShouldClose(false),
      currentRequestId(0),
      pendingRequests(),
      responseQueue(),
      responseHandler(new ResponseHandler(*this, server)),
      requestBuffer("") {
  std::cout << "[DEBUG] Client initialized with FD: " << client_fd << std::endl;
}

Client::~Client() {
  delete responseHandler;
  closeClientSocket();
}

void Client::closeClientSocket() {
  if (client_fd != -1) {
    close(client_fd);
    std::cout << "[DEBUG] Client FD " << client_fd << " closed." << std::endl;
    client_fd = -1;
  }
}

int Client::getClientFd() const { return client_fd; }

size_t Client::getBytesSent() const { return bytesSent; }

bool Client::shouldCloseConnection() const { return connectionShouldClose; }

void Client::setConnectionShouldClose(bool shouldClose) {
  connectionShouldClose = shouldClose;
  std::cout << "[DEBUG] Set connection close: " << shouldClose
            << " for client FD " << client_fd << std::endl;
}

void Client::appendToRequestBuffer(const std::string& data) {
  requestBuffer += data;
  std::cout << "[DEBUG] Appended " << data.size()
            << " bytes to request buffer for client " << client_fd << std::endl;
  processRequests();
}

void Client::processRequests() {
  size_t pos;
  while ((pos = requestBuffer.find("\r\n\r\n")) != std::string::npos) {
    std::string singleRequest = requestBuffer.substr(0, pos + 4);
    requestBuffer.erase(0, pos + 4);
    std::cout << "[DEBUG] Processing request for client FD " << client_fd
              << std::endl;
    ParseRequest parser(singleRequest);
    Request request = parser.getParsedRequest();
    pendingRequests[++currentRequestId] = std::make_pair(request, Response());
    responseHandler->handleResponse(request, currentRequestId);
  }
}

std::deque<Response> Client::getResponseQueue() const { return responseQueue; }

std::map<int, std::pair<Request, Response> > Client::getResponseMap() const {
  return pendingRequests;
}

void Client::sendResponses() {
  while (!responseQueue.empty()) {
    Response& response = responseQueue.front();
    std::string responseStr = response.toString();
    const char* responseData = responseStr.c_str();
    size_t totalSize = responseStr.size();

    std::cout << "[DEBUG] Preparing to send " << totalSize
              << " bytes to client " << client_fd << std::endl;

    while (bytesSent < totalSize) {
      ssize_t result =
          send(client_fd, responseData + bytesSent, totalSize - bytesSent, 0);
      if (result > 0) {
        bytesSent += result;
        std::cout << "[DEBUG] Sent " << result << " bytes to client "
                  << client_fd << ". Total sent: " << bytesSent << "/"
                  << totalSize << " bytes." << std::endl;
      } else if (result == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          std::cout
              << "[DEBUG] Socket not ready for sending more data for client "
              << client_fd << std::endl;
          return;
        } else {
          std::cout << "[DEBUG] send failed for FD " << client_fd << ": "
                    << strerror(errno) << std::endl;
          setConnectionShouldClose(true);
          return;
        }
      }
    }

    if (bytesSent >= totalSize) {
      responseQueue.pop_front();
      bytesSent = 0;
      std::cout << "[DEBUG] Completed sending response to client " << client_fd
                << std::endl;
      if (shouldCloseConnection()) {
        closeClientSocket();
        return;
      }
    }
  }
}

bool Client::hasPendingRequests() const { return !pendingRequests.empty(); }

void Client::enqueueResponse(const Response& response) {
  responseQueue.push_back(response);
  std::cout << "[DEBUG] Response enqueued for client FD " << client_fd
            << std::endl;
}
