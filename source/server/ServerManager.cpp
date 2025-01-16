#include "ServerManager.hpp"

ServerManager::~ServerManager() {
  std::map<int, Client*>::iterator itClients;
  for (itClients = clients.begin(); itClients != clients.end(); ++itClients) {
    delete itClients->second;
  }
}

ServerManager::ServerManager() {}

ServerManager::ServerManager(std::map<int, Port*> ports)
    : isValid(true), ports(ports), isRunning(false) {
  if (ports.empty()) {
    std::cerr << "Error: No ports provided for server manager." << std::endl;
    return;
  }
  if (!initializePorts()) {
    isValid = false;
    return;
  }
}

ServerManager::ServerManager(const ServerManager& other) { *this = other; }

ServerManager& ServerManager::operator=(const ServerManager& other) {
  if (this != &other) {
    this->ports = other.ports;
    this->isRunning = other.isRunning;
  }
  return *this;
}

bool ServerManager::good() const { return this->isValid; }

void ServerManager::start() {
  addPortsToEventReporter();
  eventReporter.run(&ServerManager::handleEvent, this);
}

bool ServerManager::initializePorts() {
  std::map<int, Port*> newPorts;
  std::map<int, Port*>::iterator itPort;

  for (itPort = ports.begin(); itPort != ports.end(); ++itPort) {
    itPort->second->initialize();
    if (!itPort->second->good()) {
      std::cerr << "Ports initialization failed." << std::endl;
      return false;
    }
    int portFd = itPort->second->getListenFd();
    newPorts[portFd] = itPort->second;
  }

  ports.swap(newPorts);
  return true;
}

void ServerManager::addPortsToEventReporter() {
  std::map<int, Port*>::iterator itPort;

  for (itPort = ports.begin(); itPort != ports.end(); ++itPort) {
    itPort->second->startListening();
    if (!itPort->second->good()) {
      return;
    }
    int portFd = itPort->second->getListenFd();
    if (!eventReporter.addFD(portFd)) {
      return;
    }
    listeningSockets.insert(itPort->second->getListenFd());
  }
}

void ServerManager::handleEvent(int fd, uint32_t events) {
  if (events & EPOLLIN) {
    handleEpollIn(fd);
  }
  if (events & EPOLLOUT) {
    handleEpollOut(fd);
  }
  if (events & EPOLLERR) {
    handleEpollOut(fd);
  }
}

void ServerManager::handleEpollIn(int listenFd) {
  if (isListeningSocket(listenFd)) {
    acceptConnection(listenFd);
  } else {
    readFromClient(listenFd);
  }
}

void ServerManager::handleEpollOut(int listenFd) {
  std::cout << "Socket " << listenFd << ": Ready to write" << std::endl;
}

void ServerManager::handleEpollErr(int listenFd) {
  std::cerr << "Socket " << listenFd << ": Error occurred" << std::endl;
}

bool ServerManager::isListeningSocket(int fd) const {
  return listeningSockets.find(fd) != listeningSockets.end();
}

void ServerManager::acceptConnection(int listenFd) {
  struct sockaddr_in clientAddr;
  socklen_t clientLen = sizeof(clientAddr);

  int clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, &clientLen);
  if (clientFd == -1) {
    std::cerr << "Client acceptation failed" << std::endl;
    return;
  }

  int flags = fcntl(clientFd, F_GETFL, 0);
  if (flags == -1 || fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) == -1) {
    std::cerr << "Failed to set non-blocking mode for clientFd" << std::endl;
    close(clientFd);
    return;
  }
  std::cout << "New client connected on fd: " << clientFd << std::endl;

  if (!eventReporter.addFD(clientFd)) {
    std::cerr << "Error: Failed to add client socket to EventReporter"
              << std::endl;
    return;
  }

  Client* client = new Client(clientFd);
  clients[clientFd] = client;
  client->setDestinationFd(listenFd);
  // clientsToPorts[clientFd] = listenFd;
}

void ServerManager::closeConnection(int clientFd) {
  if (clients.find(clientFd) != clients.end()) {
    clients[clientFd]->closeConnection();
    delete clients[clientFd];
    clients.erase(clientFd);
    // clientsToPorts.erase(clientFd);
    std::cout << "Client fd " << clientFd << " connection closed" << std::endl;
  }
}

void ServerManager::readFromClient(int clientFd) {
  if (clients.find(clientFd) == clients.end()) {
    std::cerr << "Warning: Attempt to read from a non-existing client fd "
              << clientFd << std::endl;
    return;
  }

  Client* client = clients[clientFd];
  char buffer[1024];

  ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead <= 0) {
    if (bytesRead == 0) {
      std::cout << "Client fd " << clientFd << " disconnected." << std::endl;
    } else {
      std::cerr << "Read error" << std::endl;
    }
    closeConnection(clientFd);
    return;
  }
  buffer[bytesRead] = '\0';
  client->appendToBuffer(buffer, bytesRead);

  // std::cout << "Data received from client fd " << clientFd << ": " <<
  // std::endl
  //           << buffer << std::endl;
  int destinationFd = client->getDestinationFd();
  ports[destinationFd]->processClientData(*client);
}
