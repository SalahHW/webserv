#include "Client.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>

static const std::string REQUEST_TERMINATOR = "\r\n\r\n";
static const std::string CONNECTION_HEADER = "Connection";
static const std::string KEEP_ALIVE = "keep-alive";
static const std::string HTTP_VERSION = "HTTP/1.1";

Client::Client(int client_fd, const Server &server)
    : client_fd(client_fd),
      connectionShouldClose(false),
      bytesSent(0),
      server(server) {}

Client::~Client() { closeClientSocket(); }

Client::Client(const Client &other)
    : client_fd(other.client_fd), request(other.request), server(other.server) {
  std::cout << "Client copié avec le descripteur : " << client_fd << std::endl;
}

// Client& Client::operator=(const Client& other)
// {
// return *this;
// }

void Client::closeClientSocket() {
  if (client_fd != -1) {
    close(client_fd);
    client_fd = -1;
  }
}

const int &Client::getClientFd() const { return client_fd; }
bool Client::shouldCloseConnection() const { return connectionShouldClose; }
void Client::setConnectionShouldClose(bool shouldClose) {
  connectionShouldClose = shouldClose;
}

void Client::appendToRequestBuffer(const std::string &data) {
  requestBuffer += data;
  if (isRequestComplete()) {
    processRequest();
    requestBuffer.clear();
  }
}

bool Client::isRequestComplete() const {
  return requestBuffer.find(REQUEST_TERMINATOR) != std::string::npos;
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
  HttpStatusCodeDeterminer determine(server);
  determine.determineStatusCode(request);
}

void Client::handleResponse() {
  ParseRequest parser(requestBuffer);
  RequestParsed &requestParsed = parser.getParsedRequest();

  ResponseHandler responseHandler(requestParsed, server);
  responseHandler.handleResponse();

  setResponse(responseHandler.getResponse());
}

void Client::prepareForSending() {
  bytesSent = 0;
  checkConnectionPersistence();
}

void Client::setResponse(const std::string &response) {
  responseBuffer = response;
  std::cout << "Réponse configurée : " << responseBuffer << std::endl;
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
    std::cout << "Envoyé " << result << " octets au client " << client_fd
              << std::endl;
  } else if (result == -1 && (errno != EAGAIN && errno != EWOULDBLOCK)) {
    handleError("send");
  }
  return result;
}

void Client::handleError(const std::string &functionName) {
  std::cerr << "Erreur dans " << functionName << ": " << strerror(errno)
            << std::endl;
}
