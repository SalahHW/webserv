#include "Client.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

#include "ParseRequest.hpp"
#include "ResponseHandler.hpp"

static const std::string REQUEST_TERMINATOR = "\r\n\r\n";
static const std::string CONNECTION_HEADER = "Connection";
static const std::string KEEP_ALIVE = "keep-alive";
static const std::string HTTP_VERSION = "HTTP/1.1";

Client::~Client() { closeClientSocket(); }

Client::Client(int client_fd, const Server& server)
    : client_fd(client_fd),
      connectionShouldClose(false),
      bytesSent(0),
      server(server) {}

void Client::closeClientSocket() {
  if (client_fd != -1) {
    close(client_fd);
    client_fd = -1;
  }
}

int Client::getClientFd() const { return client_fd; }
bool Client::shouldCloseConnection() const { return !request.keepAlive; }
void Client::setConnectionShouldClose(bool shouldClose) {
  connectionShouldClose = shouldClose;
}

void Client::processRequest() {
  parseRequest();
  handleResponse();
  prepareForSending();
}

void Client::parseRequest() {
  ParseRequest parser(requestBuffer);
  request = parser.getParsedRequest();
  connectionShouldClose = false;
  determineStatusCode();
}

void Client::determineStatusCode() {
  HttpStatusCodeDeterminer determiner(server);
  determiner.determineStatusCode(request);
}

void Client::handleResponse() {
  ResponseHandler responseHandler(request, server);
  responseHandler.handleResponse();
  setResponse(responseHandler.getResponse());
}

void Client::prepareForSending() {
  bytesSent = 0;
  checkConnectionPersistence();
}

void Client::appendToRequestBuffer(const std::string& data) {
  requestBuffer += data;
  std::cout << "[DEBUG] Appended " << data.size()
            << " bytes to request buffer for client " << client_fd << std::endl;
  if (isRequestComplete()) {
    std::cout << "[INFO] Complete request received from client " << client_fd
              << ". Processing request." << std::endl;
    processRequest();
    requestBuffer.clear();
  }
}

bool Client::isRequestComplete() const {
  return requestBuffer.find(REQUEST_TERMINATOR) != std::string::npos;
}

void Client::setResponse(const std::string& response) {
  responseBuffer = response;
}

void Client::checkConnectionPersistence() {
  std::map<std::string, std::string>::const_iterator it =
      request.headers.find(CONNECTION_HEADER);
  if (it != request.headers.end() && it->second == KEEP_ALIVE &&
      request.version == HTTP_VERSION) {
    setConnectionShouldClose(false);
  } else {
    setConnectionShouldClose(true);
  }
}

bool Client::hasDataToWrite() const {
  return bytesSent < responseBuffer.size();
}

ssize_t Client::sendData() {
  ssize_t result = send(client_fd, responseBuffer.c_str() + bytesSent,
                        responseBuffer.size() - bytesSent, 0);
  if (result > 0) {
    bytesSent += result;
    std::cout << "[DEBUG] Sent " << result << " bytes to client " << client_fd
              << ". Total sent: " << bytesSent << "/" << responseBuffer.size()
              << " bytes." << std::endl;
    if (bytesSent > responseBuffer.size()) {
      std::cerr << "[ERROR] bytesSent (" << bytesSent
                << ") exceeds responseBuffer size (" << responseBuffer.size()
                << ") for client " << client_fd << std::endl;
      connectionShouldClose = true;
    }
  } else if (result == -1) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::cerr << "[ERROR] send failed for FD " << client_fd << ": "
                << strerror(errno) << std::endl;
      connectionShouldClose = true;
    }
  } else {
    // result == 0, connection closed by client
    std::cout << "[INFO] Client " << client_fd
              << " closed the connection during send." << std::endl;
    connectionShouldClose = true;
  }
  return result;
}

void Client::handleError(const std::string& functionName) {
  std::cerr << "Error in " << functionName << ": " << strerror(errno)
            << std::endl;
}
