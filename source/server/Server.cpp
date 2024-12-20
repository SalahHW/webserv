#include "Server.hpp"

#include "Location.hpp"
#include "ServerHandler.hpp"

Server::~Server() {}

Server::Server() {
  this->hasListenFd = false;
  this->hasPort = false;
  this->hasName = false;
  this->hasClientMaxBodySize = false;
  this->hasErrorPages = false;
  this->hasLocations = false;
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
    this->clientsList = src.clientsList;
    this->addr = src.addr;
    this->hasListenFd = src.hasListenFd;
    this->hasPort = src.hasPort;
    this->hasName = src.hasName;
    this->hasClientMaxBodySize = src.hasClientMaxBodySize;
    this->hasErrorPages = src.hasErrorPages;
    this->hasLocations = src.hasLocations;
  }
  return *this;
}

void Server::setListenFd() {
  resolveHostName();
  this->listenFd = socket(AF_INET, SOCK_STREAM, 0);
  int optval = 1;
  if (this->listenFd == -1) throw SocketException("socket");
  if (setsockopt(this->listenFd, SOL_SOCKET, SO_REUSEADDR, &optval,
                 sizeof(optval)) == -1)
    throw SocketException("setsockopt");
  memset(&this->addr, 0, sizeof(this->addr));
  this->addr.sin_family = AF_INET;
  this->addr.sin_port = htons(this->port);
  this->addr.sin_addr.s_addr = INADDR_ANY;
}

void Server::setPort(int port) {
  this->port = port;
  this->hasPort = true;
}

void Server::setClientMaxBodySize(int size) {
  this->clientMaxBodySize = size;
  this->hasClientMaxBodySize = true;
}

void Server::setName(const std::string &name) {
  this->name = name;
  this->hasName = true;
}

void Server::setErrorPages(std::map<int, std::string> errorPages) {
  this->errorPages = errorPages;
  this->hasErrorPages = true;
}

void Server::addLocation(const Location &location) {
  this->locations[location.getPath()] = location;
  this->hasLocations = true;
}

void Server::addErrorPage(int errorCode, std::string errorPath) {
  errorPages[errorCode] = errorPath;
  this->hasErrorPages = true;
}

int Server::getListenFd() const { return this->listenFd; }

int Server::getPort() const { return this->port; }

int Server::getClientMaxBodySize() const { return this->clientMaxBodySize; }

const std::string &Server::getName() const { return this->name; }

const std::map<int, std::string> &Server::getErrorPages() const {
  return this->errorPages;
}

std::map<int, Client *> &Server::getClientsList() { return this->clientsList; }

std::map<std::string, Location> &Server::getLocations() {
  return this->locations;
}

const std::map<std::string, Location> &Server::getLocations() const {
  return this->locations;
}

void Server::displayServerInfo() const {
  std::cout << "Server Information:" << std::endl;
  if (this->hasListenFd)
    std::cout << "- Listen File Descriptor: " << listenFd << std::endl;
  if (this->hasPort) std::cout << "- Port: " << port << std::endl;
  if (this->hasName) std::cout << "- Name: " << name << std::endl;
  if (this->hasClientMaxBodySize)
    std::cout << "- Client Max Body Size: " << clientMaxBodySize << " bytes"
              << std::endl;
  if (this->hasErrorPages) {
    std::map<int, std::string>::const_iterator errorPages_it =
        errorPages.begin();
    std::cout << "- Error Pages: " << std::endl;
    while (errorPages_it != errorPages.end()) {
      std::cout << "  * " << errorPages_it->first << " "
                << errorPages_it->second << std::endl;
      errorPages_it++;
    }
  }
  if (this->hasLocations) {
    std::map<std::string, Location>::const_iterator locations_it =
        locations.begin();
    std::cout << "- Locations: " << std::endl;
    while (locations_it != locations.end()) {
      locations_it->second.displayLocationInfo();
      locations_it++;
    }
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
    throw SocketException("getaddrinfo: " + std::string(gai_strerror(status)));
  }
  memcpy(&this->addr, res->ai_addr, res->ai_addrlen);
  freeaddrinfo(res);
}

void Server::bindSocket() const {
  if (bind(this->listenFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    // TODO: fix uncaught exception
    throw SocketException("bind");
}

void Server::setToListen() const {
  if (listen(this->listenFd, SOMAXCONN) == -1) throw SocketException("listen");
}

void Server::makeSocketNonBlocking() const {
  int flags = fcntl(this->listenFd, F_GETFL, 0);
  if (flags == -1) throw SocketException("fcntl");
  flags |= O_NONBLOCK;
  if (fcntl(this->listenFd, F_SETFL, flags) == -1)
    throw SocketException("fcntl");
}

void Server::paramFd() {
  try {
    setListenFd();
    bindSocket();
    makeSocketNonBlocking();
    setToListen();
  } catch (const SocketException &excp) {
    std::cerr << "Socket error: " << excp.what() << std::endl;
  }
}

void Server::addClientToServer(Client *clientToAdd) {
  this->clientsList.insert(
      std::make_pair(clientToAdd->getClientFd(), clientToAdd));
}