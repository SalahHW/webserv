#include "Server.hpp"

#include <stdio.h>

Server::Server() : listenFd(-1), port(0), clientMaxBodySize(0) {}
Server::~Server() {
  if (listenFd != -1) {
    close(listenFd);
  }
}
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

void Server::setPort(int p) { this->port = p; }
void Server::setClientMaxBodySize(int size) { this->clientMaxBodySize = size; }
void Server::setName(const std::string &n) { this->name = n; }
void Server::setErrorPages(const std::map<int, std::string> &ep) {
  this->errorPages = ep;
}
void Server::addErrorPage(int errorCode, const std::string &errorPath) {
  this->errorPages[errorCode] = errorPath;
}
void Server::addLocation(const Location &location) {
  this->locations[location.getPath()] = location;
}

int Server::getListenFd() const { return listenFd; }
int Server::getPort() const { return port; }
int Server::getClientMaxBodySize() const { return clientMaxBodySize; }
const std::string &Server::getName() const { return name; }
const std::map<int, std::string> &Server::getErrorPages() const {
  return errorPages;
}
const std::map<std::string, Location> &Server::getLocations() const {
  return locations;
}

void Server::setListenFd() {
  this->listenFd = socket(AF_INET, SOCK_STREAM, 0);
  if (this->listenFd == -1) {
    std::cerr << "Erreur socket" << std::endl;
  }

  int optval = 1;
  if (setsockopt(this->listenFd, SOL_SOCKET, SO_REUSEADDR, &optval,
                 sizeof(optval)) == -1) {
    std::cerr << "Erreur setsockopt" << std::endl;
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(this->port);
  addr.sin_addr.s_addr = INADDR_ANY;
}

void Server::bindSocket() const {
  if (bind(this->listenFd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
    std::cerr << "Erreur bind" << std::endl;
  }
}

void Server::setToListen() const {
  if (listen(this->listenFd, SOMAXCONN) == -1) {
    perror("listen");
    std::cerr << "Erreur listen" << std::endl;
  }
}

void Server::makeSocketNonBlocking() const {
  int flags = fcntl(this->listenFd, F_GETFL, 0);
  if (flags == -1) {
    std::cerr << "Erreur fcntl" << std::endl;
  }
  flags |= O_NONBLOCK;
  if (fcntl(this->listenFd, F_SETFL, flags) == -1) {
    std::cerr << "Erreur fcntl" << std::endl;
  }
}

void Server::paramFd() {
  std::cout << "Server paramFd" << std::endl;
  setListenFd();
  bindSocket();
  makeSocketNonBlocking();
  setToListen();
}

void Server::displayServerInfo() const {
  std::cout << "Server Info:" << std::endl;
  std::cout << "- Port: " << port << std::endl;
  std::cout << "- Name: " << name << std::endl;
  std::cout << "- Max Body Size: " << clientMaxBodySize << " bytes"
            << std::endl;

  std::cout << "- Error Pages:" << std::endl;
  std::map<int, std::string>::const_iterator eit = errorPages.begin();
  for (; eit != errorPages.end(); ++eit) {
    std::cout << "  " << eit->first << ": " << eit->second << std::endl;
  }

  std::cout << "- Locations:" << std::endl;
  std::map<std::string, Location>::const_iterator lit = locations.begin();
  for (; lit != locations.end(); ++lit) {
    std::cout << "  Path: " << lit->first << std::endl;
  }
}
