#include "ServerHandler.hpp"

ServerHandler::ServerHandler(std::map<int, Server> inputServers)
    : servers(inputServers) {
  initializeEpoll();

  std::map<int, Server>::iterator it = inputServers.begin();
  for (; it != inputServers.end(); ++it) {
    it->second.paramFd();
    int listenFd = it->second.getListenFd();

    if (listenFd == -1) {
      std::cerr << "Erreur : Impossible de configurer le serveur sur le port "
                << it->second.getPort() << std::endl;
      continue;
    }

    addServerToEpoll(listenFd);
    servers[listenFd] = it->second;

    it->second.displayServerInfo();
    std::cout << "Serveur configuré sur le port : " << it->second.getPort()
              << " (FD " << listenFd << ")" << std::endl;
  }
}

ServerHandler::~ServerHandler() {
  std::map<int, Server>::iterator it = servers.begin();
  for (; it != servers.end(); ++it) {
    close(it->first);
  }
  close(epollFd);
}

void ServerHandler::initializeEpoll() {
  epollFd = epoll_create1(0);
  if (epollFd == -1) {
    std::cerr << "Erreur epoll_create1 : " << strerror(errno) << std::endl;
  }
}

void ServerHandler::addServerToEpoll(int fd) {
  struct epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1) {
    std::cerr << "Erreur epoll_ctl (FD " << fd << ") : " << strerror(errno)
              << std::endl;
  } else {
    std::cout << "Serveur ajouté à epoll : FD " << fd << std::endl;
  }
}

void ServerHandler::handleNewConnection(int listenFd) {
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);

  int clientFd =
      accept(listenFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
  if (clientFd == -1) {
    std::cerr << "Erreur accept (FD " << listenFd << ") : " << strerror(errno)
              << std::endl;
    return;
  }

  std::cout << "Nouvelle connexion acceptée : FD " << clientFd << std::endl;

  fcntl(clientFd, F_SETFL, O_NONBLOCK);

  clientManager.addClient(clientFd);

  struct epoll_event event;
  event.data.fd = clientFd;
  event.events = EPOLLIN;
  epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event);
}

void ServerHandler::startListening() {
  struct epoll_event events[MAX_EVENTS];
  std::cout << "Serveur en écoute..." << std::endl;

  while (true) {
    int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
    if (numEvents == -1) {
      std::cerr << "Erreur epoll_wait : " << strerror(errno) << std::endl;
      continue;
    }

    for (int i = 0; i < numEvents; ++i) {
      int fd = events[i].data.fd;

      if (servers.find(fd) != servers.end()) {
        handleNewConnection(fd);
      } else {
        if (events[i].events & EPOLLIN) {
          clientManager.handleClientRead(fd);
        }
        if (events[i].events & EPOLLOUT) {
          clientManager.handleClientWrite();
        }
      }
    }

    clientManager.checkTimeouts();
  }
}
