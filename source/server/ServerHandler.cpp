// ServerHandler.cpp
#include "ServerHandler.hpp"

#include <errno.h>

#include <cstring>

// Destructor
ServerHandler::~ServerHandler() {
  std::cout << "[INFO] Destroying ServerHandler" << std::endl;
  close(epollFd);
}

// Constructor
ServerHandler::ServerHandler(const ConfigFile& configFile) {
  std::cout << "[INFO] Initializing ServerHandler" << std::endl;
  try {
    serversList = ConfigExtractor::extractServers(configFile);
  } catch (const std::exception& e) {
    std::cerr << "[ERROR] Failed to extract servers: " << e.what() << std::endl;
    throw;
  }

  displayServerHandlerInfo();
  initializeEpoll();
  initializeServers();
  startListening();
}

// Copy Constructor
ServerHandler::ServerHandler(const ServerHandler& other)
    : serversList(other.serversList),
      epollFd(other.epollFd),
      nbEvents(other.nbEvents) {}

// Copy Assignment Operator
ServerHandler& ServerHandler::operator=(const ServerHandler& other) {
  if (this != &other) {
    serversList = other.serversList;
    epollFd = other.epollFd;
    nbEvents = other.nbEvents;
  }
  return *this;
}

// Initialize epoll
void ServerHandler::initializeEpoll() {
  epollFd = epoll_create1(0);
  if (epollFd == -1) {
    std::cerr << "[ERROR] epoll_create1" << std::endl;
  }
  std::cout << "[DEBUG] epoll created successfully with fd: " << epollFd
            << std::endl;
}

// Initialize Servers and add to epoll
void ServerHandler::initializeServers() {
  std::cout << "[INFO] Initializing servers..." << std::endl;
  for (std::map<int, Server>::iterator it = serversList.begin();
       it != serversList.end(); ++it) {
    addToEpoll(it->first);
    std::cout << "[DEBUG] Server with listen FD " << it->first
              << " added to epoll." << std::endl;
  }
}

// Display Server Information
void ServerHandler::displayServerHandlerInfo() const {
  std::cout << "[INFO] Displaying ServerHandler Information:" << std::endl;
  for (std::map<int, Server>::const_iterator it = serversList.begin();
       it != serversList.end(); ++it) {
    it->second.displayServerInfo();
  }
}

// Add a file descriptor to epoll
void ServerHandler::addToEpoll(int fdToAdd) const {
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;  // Edge-triggered
  event.data.fd = fdToAdd;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fdToAdd, &event) == -1) {
    std::cerr << "[ERROR] epoll_ctl ADD" << std::endl;
  }
  std::cout << "[DEBUG] FD " << fdToAdd
            << " added to epoll with EPOLLIN | EPOLLET." << std::endl;
}

// Start listening for events
void ServerHandler::startListening() {
  struct epoll_event events[MAX_EVENTS];
  std::cout << "[INFO] Starting to listen for events..." << std::endl;

  while (true) {
    nbEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
    if (nbEvents == -1) {
      std::cerr << "[ERROR] epoll_wait" << std::endl;
      continue;  // Optionally, handle fatal errors differently
    }

    std::cout << "[DEBUG] Number of events received: " << nbEvents << std::endl;

    for (int i = 0; i < nbEvents; ++i) {
      int currentFd = events[i].data.fd;
      uint32_t eventFlags = events[i].events;

      // Check if the event is on a listening socket
      std::map<int, Server>::iterator serverIt = serversList.find(currentFd);
      if (serverIt != serversList.end()) {
        std::cout << "[INFO] New connection event on listening FD: "
                  << currentFd << std::endl;
        handleNewConnection(serverIt->second);
        continue;
      }

      // Handle client events
      if (eventFlags & EPOLLIN) {
        std::cout << "[DEBUG] EPOLLIN event for FD: " << currentFd << std::endl;
        handleClientRead(currentFd);
      }

      if (eventFlags & EPOLLOUT) {
        std::cout << "[DEBUG] EPOLLOUT event for FD: " << currentFd
                  << std::endl;
        handleClientWrite(currentFd);
      }

      if (eventFlags & (EPOLLHUP | EPOLLERR)) {
        std::cerr << "[ERROR] EPOLLHUP or EPOLLERR on FD: " << currentFd
                  << std::endl;
        closeClientConnection(currentFd);
      }
    }
  }
}

// Handle new incoming connections
void ServerHandler::handleNewConnection(Server& server) {
  struct sockaddr_in clientAddr;
  socklen_t clientLen = sizeof(clientAddr);
  int clientFd =
      accept(server.getListenFd(), (struct sockaddr*)&clientAddr, &clientLen);

  if (clientFd == -1) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::cerr << "[ERROR] accept" << std::endl;
    }
    return;
  }

  // Set client socket to non-blocking
  int flags = fcntl(clientFd, F_GETFL, 0);
  if (flags == -1) {
    std::cerr << "[ERROR] fcntl GETFL" << std::endl;
    close(clientFd);
    return;
  }
  if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) == -1) {
    std::cerr << "[ERROR] fcntl SETFL" << std::endl;
    close(clientFd);
    return;
  }

  // Set socket buffer sizes
  int recvBufSize = 65536;  // 64KB
  int sendBufSize = 65536;  // 64KB
  if (setsockopt(clientFd, SOL_SOCKET, SO_RCVBUF, &recvBufSize,
                 sizeof(recvBufSize)) == -1) {
    std::cerr << "[ERROR] setsockopt SO_RCVBUF" << std::endl;
    close(clientFd);
    return;
  }
  if (setsockopt(clientFd, SOL_SOCKET, SO_SNDBUF, &sendBufSize,
                 sizeof(sendBufSize)) == -1) {
    std::cerr << "[ERROR] setsockopt SO_SNDBUF" << std::endl;
    close(clientFd);
    return;
  }

  // Create and add client to server's client list
  Client* client = new Client(clientFd, server);
  server.addClientToServer(client);
  std::cout << "[DEBUG] New client connected with FD: " << clientFd
            << std::endl;

  // Add client FD to epoll
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;
  event.data.fd = clientFd;
  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
    std::cerr << "[ERROR] epoll_ctl ADD client" << std::endl;
    close(clientFd);
    server.getClientsList().erase(clientFd);
    delete client;
    return;
  }
  std::cout << "[DEBUG] Client FD " << clientFd
            << " added to epoll with EPOLLIN | EPOLLET." << std::endl;
}

// Handle reading from a client
void ServerHandler::handleClientRead(int clientFd) {
  std::cout << "[INFO] Handling read for client FD: " << clientFd << std::endl;
  char buffer[4096];
  ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

  if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    std::string data(buffer);
    std::cout << "[DEBUG] Received data from client " << clientFd << ":\n"
              << data << std::endl;

    Client* client = findClientByFd(clientFd);
    if (client) {
      client->appendToRequestBuffer(data);

      if (client->hasDataToWrite()) {
        modifyEpollEvent(clientFd, EPOLLIN | EPOLLOUT | EPOLLET);
        std::cout << "[DEBUG] Modified epoll events for client " << clientFd
                  << " to include EPOLLOUT." << std::endl;
      }
    } else {
      std::cerr << "[ERROR] Client not found for FD: " << clientFd << std::endl;
    }
  } else if (bytesRead == 0) {
    std::cout << "[INFO] Client " << clientFd << " closed the connection."
              << std::endl;
    closeClientConnection(clientFd);
  } else {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::cerr << "[ERROR] recv" << std::endl;
      closeClientConnection(clientFd);
    }
  }
}

// Handle writing to a client
void ServerHandler::handleClientWrite(int clientFd) {
  std::cout << "[INFO] Handling write for client FD: " << clientFd << std::endl;
  Client* client = findClientByFd(clientFd);
  if (client && client->hasDataToWrite()) {
    ssize_t result = client->sendData();
    if (result > 0) {
      if (!client->hasDataToWrite()) {
        std::cout << "[DEBUG] All data sent to client " << clientFd
                  << std::endl;
        if (client->shouldCloseConnection()) {
          std::cout << "[INFO] Closing connection for client " << clientFd
                    << std::endl;
          closeClientConnection(clientFd);
        } else {
          modifyEpollEvent(clientFd, EPOLLIN | EPOLLET);
          std::cout << "[DEBUG] Modified epoll events for client " << clientFd
                    << " to EPOLLIN | EPOLLET." << std::endl;
        }
      }
    } else if (result == 0) {
      std::cout << "[INFO] Client " << clientFd
                << " closed the connection during write." << std::endl;
      closeClientConnection(clientFd);
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        std::cout
            << "[DEBUG] Non-blocking socket and send would block for client "
            << clientFd << std::endl;
      } else {
        std::cerr << "[ERROR] Error sending data to client " << clientFd << ": "
                  << strerror(errno) << std::endl;
        closeClientConnection(clientFd);
      }
    }
  } else {
    std::cerr << "[ERROR] Client not found or no data to write for FD: "
              << clientFd << std::endl;
    closeClientConnection(clientFd);
  }
}

// Close client connection and clean up
void ServerHandler::closeClientConnection(int clientFd) {
  std::cout << "[INFO] Closing connection for client FD: " << clientFd
            << std::endl;

  if (epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, NULL) == -1) {
    std::cerr << "[ERROR] Failed to remove client FD " << clientFd
              << " from epoll: " << strerror(errno) << std::endl;
  } else {
    std::cout << "[DEBUG] FD " << clientFd << " removed from epoll."
              << std::endl;
  }

  if (close(clientFd) == -1) {
    std::cerr << "[ERROR] Failed to close client FD " << clientFd << ": "
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
                << " removed from server's client list." << std::endl;
      break;
    }
  }
}

// Find a client by its file descriptor
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
  std::cerr << "[ERROR] Client not found for FD: " << clientFd << std::endl;
  return NULL;
}

// Modify epoll events for a file descriptor
void ServerHandler::modifyEpollEvent(int fd, uint32_t events) {
  struct epoll_event event;
  event.events = events;
  event.data.fd = fd;
  if (epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event) == -1) {
    std::cerr << "[ERROR] epoll_ctl MOD" << std::endl;
  } else {
    std::cout << "[DEBUG] Modified epoll events for FD " << fd << " to "
              << events << std::endl;
  }
}
