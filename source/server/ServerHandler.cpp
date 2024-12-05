#include "ServerHandler.hpp"

#include "Server.hpp"
#include "CgiHandler.hpp"

ServerHandler::~ServerHandler() {
  std::cout << "Destruction de ServerHandler" << std::endl;
  close(epollFd);
}

ServerHandler::ServerHandler(const ConfigFile &configFile) {
  std::cout << "Initialisation de ServerHandler" << std::endl;
  serversList = ConfigExtractor::extractServers(configFile);
  displayServerHandlerInfo();
  startToListen();
}

ServerHandler::ServerHandler(const ServerHandler &other)
    : serversList(other.serversList),
      epollFd(other.epollFd),
      nbEvents(other.nbEvents) {}

ServerHandler &ServerHandler::operator=(const ServerHandler &other) {
  if (this != &other) {
    serversList = other.serversList;
    epollFd = other.epollFd;
    nbEvents = other.nbEvents;
  }
  return *this;
}

void ServerHandler::displayServerHandlerInfo() const {
  std::map<int, Server>::const_iterator it;

  std::cout << "Server Handler Information:" << std::endl;
  for (it = serversList.begin(); it != serversList.end(); ++it) {
    it->second.displayServerInfo();
  }
}

void ServerHandler::serversStart() {
  std::cout << "Démarrage des serveurs..." << std::endl;
  std::map<int, Server>::iterator it;
  for (it = serversList.begin(); it != serversList.end(); ++it) {
    addToEpoll(it->first);
  }
}

void ServerHandler::epollInit() {
  this->epollFd = epoll_create1(0);
  if (this->epollFd == -1) {
    perror("epoll_create1");
  }
  std::cout << "epoll créé avec succès, descripteur : " << epollFd << std::endl;
}

void ServerHandler::addToEpoll(int fdToAdd) const {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = fdToAdd;
  if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, fdToAdd, &event) == -1) {
    throw EpollException("epoll_ctl");
  }
}

void ServerHandler::handleClientWrite(int clientFd) {
  std::cout << "Gérer l'écriture pour le client : " << clientFd << std::endl;
  Client *client = findClientByFd(clientFd);
  if (client && client->hasDataToWrite()) {
    ssize_t result = client->sendData();
    if (result == -1) {
      std::cerr << "Erreur lors de l'envoi des données au client " << clientFd
                << std::endl;
      closeClientConnection(clientFd);
    } else if (!client->hasDataToWrite()) {
      std::cout << "Toutes les données ont été envoyées au client " << clientFd
                << std::endl;
      if (client->shouldCloseConnection()) {
        std::cout << "Fermeture de la connexion pour le client " << clientFd
                  << std::endl;
        closeClientConnection(clientFd);
      } else {
        std::cout << "Modification des événements epoll pour surveiller "
                     "uniquement EPOLLIN pour le client "
                  << clientFd << std::endl;
        modifyEpollEvent(clientFd, EPOLLIN | EPOLLONESHOT | EPOLLET);
      }
    }
  } else {
    std::cerr
        << "Client non trouvé ou aucune donnée à écrire pour le descripteur : "
        << clientFd << std::endl;
  }
}

void ServerHandler::handleClientRead(int clientFd) {
  std::cout << "Gérer la lecture pour le client : " << clientFd << std::endl;
  char buffer[4096];
  ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    std::string data(buffer);
    std::cout << "Données reçues du client " << clientFd << " :\n"
              << data << std::endl;

    Client *client = findClientByFd(clientFd);
    if (client) {
      client->appendToRequestBuffer(data);

      if (client->hasDataToWrite()) {
        modifyEpollEvent(clientFd, EPOLLIN | EPOLLOUT | EPOLLET);
        std::cout << "Modification des événements epoll pour le client "
                  << clientFd << " pour surveiller EPOLLOUT." << std::endl;
      }
    } else {
      std::cerr << "Client non trouvé pour le descripteur : " << clientFd
                << std::endl;
    }
  } else if (bytesRead == 0) {
    std::cout << "Le client " << clientFd << " a fermé la connexion."
              << std::endl;
    closeClientConnection(clientFd);
  } else {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      perror("recv");
      closeClientConnection(clientFd);
    }
  }
}

void ServerHandler::handleNewConnection(Server &server) {
  int clientFd;
  struct sockaddr_in clientAddr;
  socklen_t clientLen = sizeof(clientAddr);

  clientFd =
      accept(server.getListenFd(), (struct sockaddr *)&clientAddr, &clientLen);
  if (clientFd == -1) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      perror("accept");
    }
    return;
  }

  int flags = fcntl(clientFd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl F_GETFL");
    close(clientFd);
    return;
  }
  if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror("fcntl F_SETFL");
    close(clientFd);
    return;
  }

  int recvBufSize = 65536;  // 64KB
  int sendBufSize = 65536;  // 64KB
  if (setsockopt(clientFd, SOL_SOCKET, SO_RCVBUF, &recvBufSize,
                 sizeof(recvBufSize)) == -1) {
    perror("setsockopt SO_RCVBUF");
    close(clientFd);
    return;
  }
  if (setsockopt(clientFd, SOL_SOCKET, SO_SNDBUF, &sendBufSize,
                 sizeof(sendBufSize)) == -1) {
    perror("setsockopt SO_SNDBUF");
    close(clientFd);
    return;
  }

  Client *client = new Client(clientFd, server);
  server.addClientToServer(client);

  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;
  event.data.fd = clientFd;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
    perror("epoll_ctl EPOLL_CTL_ADD");
    close(clientFd);
    server.getClientsList().erase(clientFd);
    delete client;
    return;
  }
}

void ServerHandler::closeClientConnection(int clientFd) {
  std::cout << "Fermeture de la connexion pour le client : " << clientFd
            << std::endl;

  if (epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, NULL) == -1) {
    perror("epoll_ctl DEL");
  } else {
    std::cout << "Descripteur " << clientFd << " supprimé de epoll."
              << std::endl;
  }

  if (close(clientFd) == -1) {
    perror("close");
  } else {
    std::cout << "Socket du client " << clientFd << " fermé." << std::endl;
  }

  for (std::map<int, Server>::iterator serverIt = serversList.begin();
       serverIt != serversList.end(); ++serverIt) {
    Server &server = serverIt->second;
    std::map<int, Client *> &clients = server.getClientsList();
    std::map<int, Client *>::iterator clientIt = clients.find(clientFd);
    if (clientIt != clients.end()) {
      delete clientIt->second;
      clients.erase(clientIt);
      std::cout << "Client " << clientFd << " supprimé de la liste des clients."
                << std::endl;
      break;
    }
  }
}

Client *ServerHandler::findClientByFd(int clientFd) {
  for (std::map<int, Server>::iterator serverIt = serversList.begin();
       serverIt != serversList.end(); ++serverIt) {
    Server &server = serverIt->second;
    std::map<int, Client *> &clients = server.getClientsList();
    std::map<int, Client *>::iterator clientIt = clients.find(clientFd);
    if (clientIt != clients.end()) {
      return (clientIt->second);
    }
  }
  std::cerr << "Client non trouvé pour le descripteur : " << clientFd
            << std::endl;
  return NULL;
}

void ServerHandler::modifyEpollEvent(int fd, unsigned int events) {
  struct epoll_event event;
  event.events = events;
  event.data.fd = fd;
  if (epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event) == -1) {
    perror("epoll_ctl MOD");
  }
}

void ServerHandler::startToListen() {
  struct epoll_event events[MAX_EVENTS];
  this->epollInit();
  this->serversStart();
  std::cout << "----------STARTING TO LISTEN----------" << std::endl;

  // CGI TEST BEGIN //

  CgiHandler testing;
  const std::string queryString = testing.genQueryString();
  const std::string requestMethod = testing.genRequestMethod();
  const std::string serverProtocol = testing.genServerProtocol();
  std::cout << queryString << " " << requestMethod << " "
   << serverProtocol << " " << testing.genContentLenght() << std::endl;

  std::vector<std::string> genEnv = testing.buildEnv();
  testing.printEnv(genEnv);

  // CGI TEST ENDING //

  std::cout << testing.genPathInfo("http://serveur.org/cgi-bin/monscript.cgi/marecherche") << std::endl;
  while (1) {
    this->nbEvents = epoll_wait(this->epollFd, events, MAX_EVENTS, -1);
    if (this->nbEvents == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }
    std::cout << "Nombre d'événements reçus : " << nbEvents << std::endl;
    for (int i = 0; i < this->nbEvents; i++) {
      int currentFd = events[i].data.fd;
      uint32_t eventFlags = events[i].events;
      std::map<int, Server>::iterator serverIt =
          this->serversList.find(currentFd);
      if (serverIt != this->serversList.end()) {
        std::cout << "Événement sur le descripteur d'écoute : " << currentFd
                  << std::endl;
        handleNewConnection(serverIt->second);
      } else {
        if (eventFlags & EPOLLIN) {
          std::cout << "Événement EPOLLIN détecté pour le descripteur : "
                    << currentFd << std::endl;
          handleClientRead(currentFd);
        }
        if (eventFlags & EPOLLOUT) {
          std::cout << "Événement EPOLLOUT détecté pour le descripteur : "
                    << currentFd << std::endl;
          handleClientWrite(currentFd);
        }
        if (eventFlags & (EPOLLHUP | EPOLLERR)) {
          std::cerr << "Erreur ou fermeture sur le descripteur : " << currentFd
                    << std::endl;
          closeClientConnection(currentFd);
        }
      }
    }
  }
}
