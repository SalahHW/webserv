#include "Client.hpp"

#include "Sender.hpp"

Client::~Client() {
  std::cout << RED << "Client fd " << connectionFd << " connection closed"
            << RESET << std::endl;
}

Client::Client(int listenFd, int connectionFd, Port* port)
    : listenFd(listenFd), connectionFd(connectionFd), associatedPort(port) {
  initEv();
  std::cout << GREEN << "New client connected on fd: " RESET << connectionFd
            << GREEN " port: " RESET << port->getPort() << std::endl;
}

void Client::initEv() {
  ev.events = EPOLLIN;
  ev.data.fd = connectionFd;
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
  return bytesRead;
}

void Client::eventToOut() {
  ev.events = EPOLLOUT;
  epoll_ctl(epollFd, EPOLL_CTL_MOD, connectionFd, &ev);
}

void Client::eventToIn() {
  ev.events = EPOLLIN;
  epoll_ctl(epollFd, EPOLL_CTL_MOD, connectionFd, &ev);
}

void Client::eventToErr() {
  ev.events = EPOLLERR;
  epoll_ctl(epollFd, EPOLL_CTL_MOD, connectionFd, &ev);
}

void Client::requestRoutine() {
  if (readFromClient() <= 0) {
    return;
  }
  if (buffer.find("\r\n\r\n")) {
    eventToOut();
    Request* request = new Request(getBuffer());
    requests.push_back(*request);
    clearBuffer();  // clear only before the \r\n\r\n and before
  }
}

void Client::responsesRoutine() {
  if (!requests.empty()) {
    for (std::deque<Request>::iterator it = requests.begin();
         it != requests.end(); ++it) {
      if (it->getIsParsed()) {
        if (it->getIsInTreatment()) {
          it->getResponse()->getResponseBuilder()->buildBody();
        } else {
          it->setIsInTreatment(true);
          it->setResponse(associatedPort->getVirtualHosts(),
                          associatedPort->getDefaultVirtualHostName());
        }
        Sender sender(*it->getResponse(), connectionFd, *it);
      }
      if (it->getIsTreated()) {
        eventToIn();
        requests.erase(it);  // delete request and you have to delete all what
        // is new in like the associated response
        break;
      }
    }
  }
}