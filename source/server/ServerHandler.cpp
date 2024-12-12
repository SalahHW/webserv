#include "ServerHandler.hpp"

#include "Client.hpp"

ServerHandler::ServerHandler(const ConfigFile& configFile) {
  std::cout << "[DEBUG] Initializing ServerHandler" << std::endl;
  serversList = ConfigExtractor::extractServers(configFile);
  // displayServerHandlerInfo();
  initializeEpoll();
  initializeServers();
}

ServerHandler::~ServerHandler() {
  std::cout << "[DEBUG] Destroying ServerHandler" << std::endl;
  close(epollFd);
}

void ServerHandler::initializeEpoll() {
  epollFd = epoll_create1(0);
  if (epollFd == -1) {
    std::cout << "[DEBUG] epoll_create failed: " << strerror(errno)
              << std::endl;
  } else {
    std::cout << "[DEBUG] epoll créé avec succès avec fd: " << epollFd
              << std::endl;
  }
}

void ServerHandler::initializeServers() {
  std::cout << "[DEBUG] Initializing servers..." << std::endl;
  for (std::map<int, Server>::iterator it = serversList.begin();
       it != serversList.end(); ++it) {
    addToEpoll(it->first);
    std::cout << "[DEBUG] Server avec listen FD " << it->first
              << " ajouté à epoll." << std::endl;
  }
}

void ServerHandler::displayServerHandlerInfo() const {
  std::cout << "[DEBUG] Displaying ServerHandler Information:" << std::endl;
  for (std::map<int, Server>::const_iterator it = serversList.begin();
       it != serversList.end(); ++it) {
    it->second.displayServerInfo();
  }
}

void ServerHandler::addToEpoll(int fdToAdd) const {
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;
  event.data.fd = fdToAdd;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fdToAdd, &event) == -1) {
    std::cout << "[DEBUG] epoll_ctl ADD failed for FD " << fdToAdd << ": "
              << strerror(errno) << std::endl;
  } else {
    std::cout << "[DEBUG] FD " << fdToAdd
              << " ajouté à epoll avec EPOLLIN | EPOLLET." << std::endl;
  }
}

void ServerHandler::startListening() {
  struct epoll_event events[MAX_EVENTS];
  std::cout << "[DEBUG] Starting to listen for events..." << std::endl;

  while (1) {
    checkClientTimeouts();

    nbEvents = epoll_wait(epollFd, events, MAX_EVENTS, 0);
    if (nbEvents == -1) {
      if (errno == EINTR) {
        std::cout << "[DEBUG] epoll_wait interrupted by signal." << std::endl;
        continue;
      }
      std::cerr << "[ERROR] epoll_wait failed: " << strerror(errno)
                << std::endl;
      break;
    }

    for (int i = 0; i < nbEvents; ++i) {
      int eventFd = events[i].data.fd;
      if (serversList.find(eventFd) != serversList.end()) {
        handleNewConnection(serversList[eventFd]);
      } else {
        Client* client = findClientByFd(eventFd);
        if (!client) {
          std::cout << "[DEBUG] Client pointer is NULL for FD "
                    << events[i].data.fd << std::endl;
          continue;
        }
        uint32_t eventFlags = events[i].events;

        if (eventFlags & EPOLLIN) {
          handleClientRead(client);
        }
        if (eventFlags & EPOLLOUT) {
          handleClientWrite(client);
        }
        if (eventFlags & (EPOLLHUP | EPOLLERR)) {
          client->notifyAndDelete();
        }
      }
    }
  }
}

void ServerHandler::handleNewConnection(Server& server) {
  struct sockaddr_in clientAddr;
  socklen_t clientLen = sizeof(clientAddr);
  int clientFd =
      accept(server.getListenFd(), (struct sockaddr*)&clientAddr, &clientLen);

  if (clientFd == -1) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::cout << "[DEBUG] accept failed: " << strerror(errno) << std::endl;
    }
    return;
  }

  int flags = fcntl(clientFd, F_GETFL, 0);
  if (flags == -1) {
    std::cout << "[DEBUG] fcntl GETFL failed: " << strerror(errno) << std::endl;
    close(clientFd);
    return;
  }
  if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) == -1) {
    std::cout << "[DEBUG] fcntl SETFL failed: " << strerror(errno) << std::endl;
    close(clientFd);
    return;
  }

  int recvBufSize = 65536;
  int sendBufSize = 65536;
  if (setsockopt(clientFd, SOL_SOCKET, SO_RCVBUF, &recvBufSize,
                 sizeof(recvBufSize)) == -1) {
    std::cout << "[DEBUG] setsockopt SO_RCVBUF failed: " << strerror(errno)
              << std::endl;
    close(clientFd);
    return;
  }
  if (setsockopt(clientFd, SOL_SOCKET, SO_SNDBUF, &sendBufSize,
                 sizeof(sendBufSize)) == -1) {
    std::cout << "[DEBUG] setsockopt SO_SNDBUF failed: " << strerror(errno)
              << std::endl;
    close(clientFd);
    return;
  }

  Client* client = new Client(clientFd, server, this);
  server.addClientToServer(client);
  std::cout << "[DEBUG] New client connected with FD: " << clientFd
            << std::endl;

  addClientToEpoll(client, server);
}

void ServerHandler::addClientToEpoll(Client* client, Server& server) {
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;
  event.data.fd = client->getClientFd();
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, client->getClientFd(), &event) == -1) {
    std::cout << "[DEBUG] epoll_ctl ADD client failed for FD "
              << client->getClientFd() << ": " << strerror(errno) << std::endl;
    close(client->getClientFd());
    server.getClientsList().erase(client->getClientFd());
    delete client;
    return;
  }
  std::cout << "[DEBUG] Client FD " << client->getClientFd()
            << " ajouté à epoll avec EPOLLIN | EPOLLET." << std::endl;
}

void ServerHandler::handleClientRead(Client* client) {
  int clientFd = client->getClientFd();
  std::cout << "[DEBUG] Handling read for client FD: " << clientFd << std::endl;

  char buffer[2048];
  while (true) {
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
      std::string data(buffer, bytesRead);
      std::cout << "[DEBUG] Received " << bytesRead << " bytes from client "
                << clientFd << std::endl;
      client->appendToRequestBuffer(data);
    } else if (bytesRead == 0) {
      std::cout << "[DEBUG] Client " << clientFd << " closed the connection."
                << std::endl;
      client->setConnectionShouldClose(true);
      break;
    } else if (bytesRead == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      } else {
        std::cout << "[DEBUG] recv failed for FD " << clientFd << ": "
                  << strerror(errno) << std::endl;
        client->notifyAndDelete();
        break;
      }
    }
  }

  if (client->hasPendingRequests()) {
    modifyEpollEvent(client, EPOLLIN | EPOLLOUT | EPOLLET);
    std::cout << "[DEBUG] Modified epoll events for client " << clientFd
              << " to include EPOLLOUT." << std::endl;
  }
}

void ServerHandler::handleClientWrite(Client* client) {
  int clientFd = client->getClientFd();
  std::cout << "[DEBUG] Handling write for client FD: " << clientFd
            << std::endl;
  client->sendResponses();
  if (client->getResponseQueue().empty()) {
    modifyEpollEvent(client, EPOLLIN | EPOLLET);
    std::cout << "[DEBUG] Modified epoll events for client " << clientFd
              << " to include EPOLLIN." << std::endl;
  }
}

void ServerHandler::removeClient(int fd) {
  if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL) == -1) {
    std::cout << "[DEBUG] epoll_ctl DEL failed for FD " << fd << ": "
              << strerror(errno) << std::endl;
  } else {
    std::cout << "[DEBUG] FD " << fd << " removed from epoll." << std::endl;
  }

  for (std::map<int, Server>::iterator serverIt = serversList.begin();
       serverIt != serversList.end(); ++serverIt) {
    Server& server = serverIt->second;
    std::map<int, Client*>& clients = server.getClientsList();
    std::map<int, Client*>::iterator clientIt = clients.find(fd);
    if (clientIt != clients.end()) {
      clients.erase(clientIt);
      std::cout << "[DEBUG] Client FD " << fd
                << " removed from the server's client list." << std::endl;
      break;
    }
  }
}

Client* ServerHandler::findClientByFd(int clientFd) {
  for (std::map<int, Server>::iterator serverIt = serversList.begin();
       serverIt != serversList.end(); ++serverIt) {
    Server& server = serverIt->second;
    std::map<int, Client*>& clients = server.getClientsList();
    std::map<int, Client*>::iterator clientIt = clients.find(clientFd);
    if (clientIt != clients.end()) {
      return clientIt->second;
    }
  }
  return NULL;
}

void ServerHandler::modifyEpollEvent(Client* client, uint32_t events) {
  struct epoll_event event;
  event.events = events;
  event.data.fd = client->getClientFd();
  if (epoll_ctl(epollFd, EPOLL_CTL_MOD, client->getClientFd(), &event) == -1) {
    std::cout << "[DEBUG] epoll_ctl MOD failed for FD " << client->getClientFd()
              << ": " << strerror(errno) << std::endl;
  } else {
    std::cout << "[DEBUG] Modified epoll events for FD "
              << client->getClientFd() << " to " << events << std::endl;
  }
}

void ServerHandler::checkClientTimeouts() {
  time_t currentTime = time(NULL);
  for (std::map<int, Server>::iterator serverIt = serversList.begin();
       serverIt != serversList.end(); ++serverIt) {
    Server& server = serverIt->second;
    std::map<int, Client*>& clients = server.getClientsList();

    for (std::map<int, Client*>::iterator clientIt = clients.begin();
         clientIt != clients.end();) {
      Client* client = clientIt->second;
      if (client->isTimedOut(currentTime, CLIENT_TIMEOUT)) {
        std::cout << "[DEBUG] Client FD " << client->getClientFd()
                  << " timed out." << std::endl;
        ++clientIt;
        client->setConnectionShouldClose(true);
        client->notifyAndDelete();
      } else {
        ++clientIt;
      }
    }
  }
}