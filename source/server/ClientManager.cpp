#include "ClientManager.hpp"

ClientManager::ClientManager()
    : requestParser(0),
      responseHandler(0),
      responseSender(0),
      requestValidator(0) {}

ClientManager::~ClientManager() {}

void ClientManager::addClient(int clientFd) {
  clients.insert(std::make_pair(clientFd, Client(clientFd)));
  std::cout << "Client ajouté : FD " << clientFd << std::endl;
}

void ClientManager::removeClient(int clientFd) {
  closeClient(clientFd);
  clients.erase(clientFd);
  std::cout << "Client supprimé : FD " << clientFd << std::endl;
}

void ClientManager::closeClient(int clientFd) {
  close(clientFd);
  std::cout << "Connexion fermée pour le client FD " << clientFd << std::endl;
}

void ClientManager::checkTimeouts() {
  time_t now = time(NULL);
  std::map<int, Client>::iterator it = clients.begin();
  while (it != clients.end()) {
    std::map<int, Client>::iterator current = it++;
    if (now - current->second.getLastActivity() > CLIENT_TIMEOUT) {
      std::cout << "Client timeout : FD " << current->first << std::endl;
      closeClient(current->first);
      clients.erase(current);
    }
  }
}

void ClientManager::handleClientRead(int clientFd) {
  char buffer[1024];
  ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);

  if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    clients[clientFd].setLastActivity(time(NULL));

    std::string rawData(buffer);
    Request request;
    try {
      if (requestParser)
        requestParser->parse(rawData, request);
      else {
        std::cerr << "No RequestParser set !" << std::endl;
        return;
      }

      Response response;
      if (requestValidator && !requestValidator->validate(request, response)) {
      } else {
        if (responseHandler) {
          response = responseHandler->handleResponse(
              request.getMethod(), request.getUri(),
              (int)request.getBody().size(), request.isKeepAlive());
        } else {
          std::cerr << "No ResponseHandler set !" << std::endl;
          return;
        }
      }

      if (responseSender) {
        responseSender->addResponse(clients[clientFd], response);
      } else {
        std::cerr << "No ResponseSender set !" << std::endl;
      }

    } catch (std::exception& e) {
      std::cerr << "Error parsing request: " << e.what() << std::endl;
    }

  } else if (bytesRead == 0) {
    std::cout << "Client fermé proprement : FD " << clientFd << std::endl;
    removeClient(clientFd);
  } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
    std::cerr << "Erreur de lecture pour FD " << clientFd << ": "
              << std::strerror(errno) << std::endl;
    removeClient(clientFd);
  }
}

void ClientManager::handleClientWrite() {
  if (responseSender) {
    responseSender->sendPendingResponses();
  }
}
