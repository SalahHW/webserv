// Server.cpp
#include "Server.hpp"

#include "Location.hpp"
#include "ServerHandler.hpp"

Server::~Server() {}

Server::Server() {}

Server::Server(const Server &src) { *this = src; }

Server &Server::operator=(const Server &src) {
  if (this != &src) {
    this->listenFd = src.listenFd;
    this->port = src.port;
    this->name = src.name;
    this->clientMaxBodySize = src.clientMaxBodySize;
    this->errorPages = src.errorPages;
    this->locations = src.locations;
  }
  return *this;
}

void Server::setListenFd() {
  resolveHostName();
  this->listenFd = socket(AF_INET, SOCK_STREAM, 0);
  int optval = 1;
  if (this->listenFd == -1) {
    std::cerr << "socket" << std::endl;
  }
  if (setsockopt(this->listenFd, SOL_SOCKET, SO_REUSEADDR, &optval,
                 sizeof(optval)) == -1) {
    std::cerr << "setsockopt" << std::endl;
  }
  memset(&this->addr, 0, sizeof(this->addr));
  this->addr.sin_family = AF_INET;
  this->addr.sin_port = htons(this->port);
  this->addr.sin_addr.s_addr = INADDR_ANY;
}

void Server::setPort(int port) { this->port = port; }

void Server::setClientMaxBodySize(int size) { this->clientMaxBodySize = size; }

void Server::setName(const std::string &name) { this->name = name; }

void Server::setErrorPages(std::map<int, std::string> errorPages) {
  this->errorPages = errorPages;
  // debug
  this->errorPages[PAGE_NOT_FOUND] = "./www/errorsPages/404.html";
  std::cout << "ERROR PATH IN MAP= " << errorPages[PAGE_NOT_FOUND] << std::endl;
}

void Server::addLocation(const Location &location) {
  this->locations[location.getPath()] = location;
}

int Server::getListenFd() const { return this->listenFd; }

int Server::getPort() const { return this->port; }

int Server::getClientMaxBodySize() const { return this->clientMaxBodySize; }

const std::string &Server::getName() const { return this->name; }

const std::map<int, std::string> &Server::getErrorPages() const {
  return this->errorPages;
}

std::map<int, Client *> &Server::getClientsList() { return this->clientsList; }

std::map<std::string, Location> Server::getLocations() const {
  return this->locations;
}

void Server::displayServerInfo() const {
  std::cout << "Server Information:" << std::endl;
  std::cout << "- Listen File Descriptor: " << listenFd << std::endl;
  std::cout << "- Port: " << port << std::endl;
  std::cout << "- Name: " << name << std::endl;
  std::cout << "- Client Max Body Size: " << clientMaxBodySize << " bytes"
            << std::endl;

  // std::cout << "- Error Pages: " << std::endl;
  // for (size_t i = 0; i < errorPages.size(); ++i) {
  //      std::cout << "  * " << errorPages[i] << std::endl;
  // }

  std::map<std::string, Location>::const_iterator it = this->locations.begin();
  std::cout << "- Locations: " << std::endl;
  for (; it != this->locations.end(); ++it) {
    it->second.displayLocationInfo();
  }
}

void Server::resolveHostName() {
  struct addrinfo hints, *res;
  int status;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  char port_str[6];
  sprintf(port_str, "%d", this->port);

  status = getaddrinfo(this->name.c_str(), port_str, &hints, &res);
  if (status != 0) {
    std::cout << "getaddrinfo: " + std::string(gai_strerror(status))
              << std::endl;
  }
  memcpy(&this->addr, res->ai_addr, res->ai_addrlen);
  freeaddrinfo(res);
}

void Server::bindSocket() const {
  if (bind(this->listenFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    std::cerr << "bind" << std::endl;
}

void Server::setToListen() const {
  if (listen(this->listenFd, SOMAXCONN) == -1) {
    std::cerr << "listen" << std::endl;
  }
}

void Server::makeSocketNonBlocking() const {
  int flags = fcntl(this->listenFd, F_GETFL, 0);
  if (flags == -1) {
    std::cerr << "fcntl" << std::endl;
  }
  flags |= O_NONBLOCK;
  if (fcntl(this->listenFd, F_SETFL, flags) == -1) {
    std::cerr << "fcntl" << std::endl;
  }
}

void Server::paramFd() {
  setListenFd();
  bindSocket();
  makeSocketNonBlocking();
  setToListen();
}

void Server::addClientToServer(Client *clientToAdd) {
  this->clientsList.insert(
      std::make_pair(clientToAdd->getClientFd(), clientToAdd));
}