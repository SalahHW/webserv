#include "Client.hpp"

#include "Sender.hpp"

Client::~Client() {
  std::cout << RED << "Client fd " << connectionFd << " connection closed"
            << RESET << std::endl;
}

Client::Client(int listenFd, int connectionFd, Port* port)
    : status(WAITING),
      listenFd(listenFd),
      connectionFd(connectionFd),
      associatedPort(port) {
  std::cout << GREEN << "New client connected on fd: " RESET << connectionFd
            << GREEN " port: " RESET << port->getPort() << std::endl;
}

std::string& Client::getBuffer() { return buffer; }

int Client::getListenFd() const { return this->listenFd; }

int Client::getConnectionFd() const { return this->connectionFd; }

void Client::closeConnection() {
  close(connectionFd);
  clearBuffer();
}

void Client::appendToBuffer(const char* data, size_t len) {
  buffer.append(data, len);
  status = PROCESSING;
}

void Client::clearBuffer() { buffer.clear(); }

int Client::readFromClient() {
  char buffer[1024];

  ssize_t bytesRead = recv(connectionFd, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead <= 0) {
    if (bytesRead < 0)
      std::cerr << "Read error on client fd " << connectionFd << std::endl;
    closeConnection();
    return bytesRead;
  }
  buffer[bytesRead] = '\0';
  appendToBuffer(buffer, bytesRead);
  //  Status = Have work
  std::cout << "Coucou" << std::endl;
  return bytesRead;
}

void Client::executeNextTask() {
  std::cout << "TEST" << std::endl;
  if (!requests.empty()) {
    for (std::deque<Request>::iterator it = requests.begin();
         it != requests.end(); ++it) {
      if (it->getIsParsed()) {
        // init response
        it->setResponse(associatedPort->getVirtualHosts(),
                        associatedPort->getDefaultVirtualHostName());
      }
      if (it->getIsInTreatment()) {
        // buildResponse
        it->getResponse()->getResponseBuilder().buildBody();
      }
      if (it->getIsTreated()) {
        // delete the request and associated response
      }
    }
  }
  if (buffer.empty()) return;
  if (buffer.find("\r\n\r\n")) {
  }
}