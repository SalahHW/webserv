#include "ServerManager.hpp"

ServerManager* ServerManager::instance = NULL;

void signalHandler(int sigNum)
{
  (void)sigNum;
  if (ServerManager::instance != NULL)
  {
    ServerManager::instance->setIsRunning(false);
  }
}

ServerManager::~ServerManager()
{
  std::map<int, Client*>::iterator itClients;
  for (itClients = clients.begin(); itClients != clients.end(); ++itClients)
  {
    delete itClients->second;
  }
}

ServerManager::ServerManager()
{
  ServerManager::instance = this;
  signal(SIGINT, signalHandler);
}

void ServerManager::setIsRunning(bool isRunning) { this->isRunning = isRunning; }

ServerManager::ServerManager(std::map<int, Port*> ports)
    : isValid(true)
    , ports(ports)
    , isRunning(false)
{
  ServerManager::instance = this;
  signal(SIGINT, signalHandler);

  if (ports.empty())
  {
    std::cerr << "Error: No ports provided for server manager." << std::endl;
    return;
  }
  if (!initializePorts())
  {
    isValid = false;
    return;
  }
}

ServerManager::ServerManager(const ServerManager& other) { *this = other; }

ServerManager& ServerManager::operator=(const ServerManager& other)
{
  if (this != &other)
  {
    this->ports = other.ports;
    this->isRunning = other.isRunning;
  }
  return *this;
}

bool ServerManager::good() const { return this->isValid; }

void ServerManager::start()
{
  addPortsToEventReporter();
  isRunning = true;
  std::cout << "-----Server Started-----" << std::endl;
  runRoutine();
}

void ServerManager::stop()
{
  std::map<int, Client*>::iterator itClients = clients.begin();
  for (; itClients != clients.end(); ++itClients)
  {
    itClients->second->closeConnection();
  }
  std::cout << "-----Server Stopped-----" << std::endl;
}

void ServerManager::runRoutine()
{
  struct epoll_event events[1000];

  while (isRunning)
  {
    int eventCount = epoll_wait(eventReporter.getEpollFd(), events, 1000, -1);
    if (eventCount < 0)
    {
      perror("epoll_wait");
      if (errno == EINTR)
      {
        continue;
      }
      break;
    }

    for (int i = 0; i < eventCount; ++i)
    {
      int fd = events[i].data.fd;
      uint32_t eventFlags = events[i].events;
      if (clients.find(fd) != clients.end())
      {
        if (clients[fd]->getCurrentTime() - clients[fd]->lastActivity > TIMEOUT)
        {
          closeConnection(fd);
        }
      }
      handleEvent(fd, eventFlags);
    }
  }

  stop();
}

bool ServerManager::initializePorts()
{
  std::map<int, Port*> newPorts;
  std::map<int, Port*>::iterator itPort;

  for (itPort = ports.begin(); itPort != ports.end(); ++itPort)
  {
    itPort->second->initialize();
    if (!itPort->second->good())
    {
      std::cerr << "Ports initialization failed." << std::endl;
      return false;
    }
    int portFd = itPort->second->getListenFd();
    newPorts[portFd] = itPort->second;
  }

  ports.swap(newPorts);
  return true;
}

void ServerManager::addPortsToEventReporter()
{
  std::map<int, Port*>::iterator itPort;

  for (itPort = ports.begin(); itPort != ports.end(); ++itPort)
  {
    itPort->second->startListening();
    if (!itPort->second->good())
    {
      return;
    }
    int portFd = itPort->second->getListenFd();
    if (!eventReporter.addFD(portFd))
    {
      return;
    }
    listeningSockets.insert(itPort->second->getListenFd());
  }
}

void ServerManager::handleEvent(int fd, uint32_t events)
{
  if (clients.find(fd) != clients.end())
  {
    if (clients[fd]->getEvent() == EPOLLIN)
    {
      handleEpollIn(fd);
    }
    if (clients[fd]->getEvent() == EPOLLOUT)
    {
      handleEpollOut(fd);
    }
    if (clients[fd]->getEvent() == EPOLLERR || events == EPOLLHUP || events == EPOLLRDHUP)
    {
      handleEpollErr(fd);
    }
  }
  else
  {
    if (events & EPOLLIN)
    {
      handleEpollIn(fd);
    }
    if (events & EPOLLOUT)
    {
      handleEpollOut(fd);
    }
    if (events & EPOLLERR || events & EPOLLHUP || events & EPOLLRDHUP)
    {
      handleEpollErr(fd);
    }
  }
}

void ServerManager::handleEpollIn(int listenFd)
{
  if (isListeningSocket(listenFd))
  {
    //   std::cout << "Socket " << listenFd << ": Ready to accept connection"
    //             << std::endl;
    acceptConnection(listenFd);
    return;
  }
  clients.find(listenFd)->second->requestRoutine();
}

void ServerManager::handleEpollOut(int listenFd)
{
  // std::cout << "Socket " << listenFd << ": Ready to write" << std::endl;
  clients.find(listenFd)->second->responsesRoutine();
}

void ServerManager::handleEpollErr(int listenFd)
{
  closeConnection(listenFd);
}

bool ServerManager::isListeningSocket(int fd) const
{
  return listeningSockets.find(fd) != listeningSockets.end();
}

void ServerManager::acceptConnection(int listenFd)
{
  struct sockaddr_in clientAddr;
  socklen_t clientLen = sizeof(clientAddr);

  int clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, &clientLen);
  if (clientFd == -1)
  {
    std::cerr << "Client acceptation failed" << std::endl;
    return;
  }
  int flags = fcntl(clientFd, F_GETFL, 0);
  if (flags == -1 || fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) == -1)
  {
    std::cerr << "Failed to set non-blocking mode for clientFd" << std::endl;
    close(clientFd);
    return;
  }
  if (!eventReporter.addFD(clientFd))
  {
    std::cerr << "Error: Failed to add client socket to EventReporter"
              << std::endl;
    return;
  }

  Client* client = new Client(listenFd, clientFd, ports[listenFd]);
  client->epollFd = eventReporter.getEpollFd();
  clients[clientFd] = client;
}

void ServerManager::closeConnection(int clientFd)
{
  if (clients.find(clientFd) != clients.end())
  {
    epoll_ctl(eventReporter.getEpollFd(), EPOLL_CTL_DEL, clientFd, NULL);
    clients[clientFd]->closeConnection();
    delete clients[clientFd];
    clients.erase(clientFd);
  }
}
