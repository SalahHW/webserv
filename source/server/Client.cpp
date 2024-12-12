#include "Client.hpp"

#include "ResponseHandler.hpp"
#include "ServerHandler.hpp"

Client::Client(int client_fd, const Server& server, ServerHandler* handler)
    : client_fd(client_fd),
      connectionShouldClose(false),
      currentRequestId(0),
      pendingRequests(),
      responseQueue(),
      responseHandler(new ResponseHandler(*this, server)),
      requestBuffer(""),
      lastActivity(time(NULL)),
      serverHandler(handler) {
  std::cout << "[DEBUG] Client initialized with FD: " << client_fd << std::endl;
}

Client::~Client() {
  delete responseHandler;
  closeClientSocket();
}

void Client::closeClientSocket() {
  if (client_fd != -1) {
    std::cout << "[DEBUG] Client FD " << client_fd << " closed." << std::endl;
    close(client_fd);
    client_fd = -1;
  }
}

int Client::getClientFd() const { return client_fd; }

bool Client::shouldCloseConnection() const { return connectionShouldClose; }

void Client::setConnectionShouldClose(bool shouldClose) {
  connectionShouldClose = shouldClose;
  std::cout << "[DEBUG] Set connection close: " << shouldClose
            << " for client FD " << client_fd << std::endl;
}

void Client::appendToRequestBuffer(const std::string& data) {
  requestBuffer += data;
  updateLastActivity();
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
    const std::string& connectionHeader = request.getHeader("Connection");
    if (connectionHeader == "close") {
      setConnectionShouldClose(true);
    } else {
      setConnectionShouldClose(false);
    }
    pendingRequests[++currentRequestId] = std::make_pair(request, Response());
    responseHandler->handleResponse(request, currentRequestId);
    updateLastActivity();
  }
}

std::deque<Response> Client::getResponseQueue() const { return responseQueue; }

std::map<int, std::pair<Request, Response> > Client::getResponseMap() const {
  return pendingRequests;
}

bool Client::hasPendingRequests() const { return !pendingRequests.empty(); }

void Client::enqueueResponse(const Response& response) {
  Response mutableResponse = response;
  mutableResponse.initialize();
  responseQueue.push_back(mutableResponse);
  std::cout << "[DEBUG] Response enqueued for client FD " << client_fd
            << std::endl;
}

void Client::sendResponses() {
  while (!responseQueue.empty()) {
    Response& response = responseQueue.front();
    size_t totalSize =
        response.getSendData(0) ? strlen(response.getSendData(0)) : 0;
    const char* responseData = response.getSendData(totalSize);
    size_t bytesToSend = totalSize;

    if (bytesToSend == 0) {
      responseQueue.pop_front();
      continue;
    }

    std::cout << "[DEBUG] Preparing to send " << bytesToSend
              << " bytes to client " << client_fd << std::endl;

    ssize_t result = send(client_fd, responseData, bytesToSend, 0);
    if (result > 0) {
      response.updateBytesSent(result);
      std::cout << "[DEBUG] Sent " << result << " bytes to client " << client_fd
                << ". Total sent: " << response.isFullySent() << "/"
                << bytesToSend << " bytes." << std::endl;
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
        break;
      }
    }

    if (response.isFullySent()) {
      responseQueue.pop_front();
      std::cout << "[DEBUG] Completed sending response to client " << client_fd
                << std::endl;
    }
  }

  if (pendingClose && responseQueue.empty()) {
    notifyAndDelete();
  }
}

bool Client::isTimedOut(time_t currentTime, int timeoutDuration) const {
  return difftime(currentTime, lastActivity) >= timeoutDuration;
}

void Client::updateLastActivity() { lastActivity = time(NULL); }

void Client::notifyAndDelete() {
  serverHandler->removeClient(client_fd);
  std::cout << "[DEBUG] Client FD " << client_fd << " DESTROYED." << std::endl;
}