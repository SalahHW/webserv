#include "ServerHandler.hpp"

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

  while (1) {  // Boucle infinie pour écouter les événements
    nbEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
    if (nbEvents == -1) {
      if (errno == EINTR) {
        continue;  // Interruption par un signal, on relance l'attente
      }
      std::cerr << "[DEBUG] epoll_wait failed: " << strerror(errno)
                << std::endl;
      break;
    }

    for (int i = 0; i < nbEvents; ++i) {
      int currentFd = events[i].data.fd;
      uint32_t eventFlags = events[i].events;

      if (serversList.find(currentFd) != serversList.end()) {
        // Nouvelle connexion sur un FD de serveur
        handleNewConnection(serversList[currentFd]);
      } else if (eventFlags & EPOLLIN) {
        // Lecture possible
        handleClientRead(currentFd);
      } else if (eventFlags & EPOLLOUT) {
        // Écriture possible
        handleClientWrite(currentFd);
      } else if (eventFlags & (EPOLLHUP | EPOLLERR)) {
        // Erreur ou fermeture
        closeClientConnection(currentFd);
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

  Client* client = new Client(clientFd, server);
  server.addClientToServer(client);
  std::cout << "[DEBUG] New client connected with FD: " << clientFd
            << std::endl;

  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;
  event.data.fd = clientFd;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
    std::cout << "[DEBUG] epoll_ctl ADD client failed for FD " << clientFd
              << ": " << strerror(errno) << std::endl;
    close(clientFd);
    server.getClientsList().erase(clientFd);
    delete client;
    return;
  }
  std::cout << "[DEBUG] Client FD " << clientFd
            << " ajouté à epoll avec EPOLLIN | EPOLLET." << std::endl;
}

void ServerHandler::handleClientRead(int clientFd) {
  std::cout << "[DEBUG] Handling read for client FD: " << clientFd << std::endl;
  Client* client = findClientByFd(clientFd);
  if (!client) {
    std::cout << "[DEBUG] Client not found for FD: " << clientFd << std::endl;
    closeClientConnection(clientFd);
    return;
  }

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
      closeClientConnection(clientFd);
      break;
    } else if (bytesRead == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      } else {
        std::cout << "[DEBUG] recv failed for FD " << clientFd << ": "
                  << strerror(errno) << std::endl;
        closeClientConnection(clientFd);
        break;
      }
    }
  }

  if (client->hasPendingRequests()) {
    modifyEpollEvent(clientFd, EPOLLIN | EPOLLOUT | EPOLLET);
    std::cout << "[DEBUG] Modified epoll events for client " << clientFd
              << " to include EPOLLOUT." << std::endl;
  }
}

void ServerHandler::handleClientWrite(int clientFd) {
  std::cout << "[DEBUG] Handling write for client FD: " << clientFd
            << std::endl;
  Client* client = findClientByFd(clientFd);
  if (!client) {
    std::cout << "[DEBUG] Client not found for FD: " << clientFd << std::endl;
    closeClientConnection(clientFd);
    return;
  }
  client->sendResponses();
  if (client->shouldCloseConnection()) {
    std::cout << "[DEBUG] Closing connection for client " << clientFd
              << std::endl;
    closeClientConnection(clientFd);
  } else if (!client->hasPendingRequests()) {
    modifyEpollEvent(clientFd, EPOLLIN | EPOLLET);
  }
}

void ServerHandler::closeClientConnection(int clientFd) {
  std::cout << "[DEBUG] Closing connection for client FD: " << clientFd
            << std::endl;

  if (epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, NULL) == -1) {
    std::cout << "[DEBUG] epoll_ctl DEL failed for FD " << clientFd << ": "
              << strerror(errno) << std::endl;
  } else {
    std::cout << "[DEBUG] FD " << clientFd << " removed de epoll." << std::endl;
  }

  if (close(clientFd) == -1) {
    std::cout << "[DEBUG] Failed to close client FD " << clientFd << ": "
              << strerror(errno) << std::endl;
  } else {
    std::cout << "[DEBUG] Socket for client FD " << clientFd << " closed."
              << std::endl;
  }

  for (std::map<int, Server>::iterator serverIt = serversList.begin();
       serverIt != serversList.end(); ++serverIt) {
    Server& server = serverIt->second;
    std::map<int, Client*>& clients = server.getClientsList();
    std::map<int, Client*>::iterator clientIt = clients.find(clientFd);
    if (clientIt != clients.end()) {
      delete clientIt->second;
      clients.erase(clientIt);
      std::cout << "[DEBUG] Client FD " << clientFd
                << " removed de la liste des clients du serveur." << std::endl;
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

void ServerHandler::modifyEpollEvent(int fd, uint32_t events) {
  struct epoll_event event;
  event.events = events;
  event.data.fd = fd;
  if (epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event) == -1) {
    std::cout << "[DEBUG] epoll_ctl MOD failed for FD " << fd << ": "
              << strerror(errno) << std::endl;
  } else {
    std::cout << "[DEBUG] Modified epoll events for FD " << fd << " to "
              << events << std::endl;
  }
}
