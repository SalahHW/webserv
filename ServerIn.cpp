#include "ServerIn.hpp"

ServerIn::ServerIn() {
  try {
    createSocket();
    bindSocket();
    makeSocketNonBlocking();
    setToListen();
  } catch (const SocketException &excp) {
    std::cerr << "Socket error: " << excp.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}

ServerIn::~ServerIn() {
  // close(this->listen_sock_fd);
}

ServerIn::ServerIn(const ServerIn &src)
    : listen_sock_fd(src.listen_sock_fd), addr(src.addr) {}

ServerIn &ServerIn::operator=(const ServerIn &src) {
  this->listen_sock_fd = src.listen_sock_fd;
  return *this;
}

int ServerIn::getListenSockFd() { return this->listen_sock_fd; }

struct sockaddr_in ServerIn::getAddr() {
  return this->addr;
}

void ServerIn::createSocket() {
  this->listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  int optval = 1;
  if (this->listen_sock_fd == -1)
    throw SocketException("socket");
  if (setsockopt(this->listen_sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval,
                 sizeof(optval)) == -1)
    throw SocketException("setsockopt");
  memset(&this->addr, 0, sizeof(this->addr));
  this->addr.sin_family = AF_INET;
  this->addr.sin_port = htons(PORT);
  this->addr.sin_addr.s_addr = INADDR_ANY;
}

void ServerIn::bindSocket() {
  if (bind(this->listen_sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    throw SocketException("bind");
}

void ServerIn::setToListen() {
  if (listen(this->listen_sock_fd, SOMAXCONN) == -1)
    throw SocketException("listen");
}

void ServerIn::makeSocketNonBlocking() {
  int flags = fcntl(this->listen_sock_fd, F_GETFL, 0);
  if (flags == -1)
    throw SocketException("fcntl");
  flags |= O_NONBLOCK;
  if (fcntl(this->listen_sock_fd, F_SETFL, flags) == -1)
    throw SocketException("fcntl");
}