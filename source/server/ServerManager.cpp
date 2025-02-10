#include "ServerManager.hpp"

ServerManager* ServerManager::instance = NULL;

void signalHandler(int signum)
{
  (void)signum;
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

ServerManager::ServerManager(const ServerManager& other)
{
  *this = other;
}

ServerManager& ServerManager::operator=(const ServerManager& other)
{
  if (this != &other)
  {
    this->ports = other.ports;
    this->isRunning = other.isRunning;
  }
  return *this;
}

bool ServerManager::good() const
{
  return this->isValid;
}

void ServerManager::start()
{
  addPortsToEventReporter();
  isRunning = true;
  runRoutine();
}

void ServerManager::stop()
{
  // TODO: Close all connexions
  std::cout << "Server stopping..." << std::endl;
}

void ServerManager::runRoutine()
{
  while (isRunning)
  {
    uint32_t eventFlags;
    int fd = eventReporter.getNextEvent(eventFlags);

    if (fd == -1)
    {
      if (!eventReporter.good())
        break;
      continue;
    }
    handleEvent(fd, eventFlags);
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

// TODO: Simplifier en cherchant directement si le client existe deja
void ServerManager::handleEpollIn(int fd)
{
  if (isListeningSocket(fd))
  {
    acceptConnection(fd);
  }
  else
  {
    processClientInput(fd);
  }
}

void ServerManager::handleEpollOut(int listenFd)
{
  std::cout << "Socket " << listenFd << ": Ready to write" << std::endl;
}

void ServerManager::handleEpollErr(int listenFd)
{
  std::cerr << "Socket " << listenFd << ": Error occurred" << std::endl;
}

bool ServerManager::isListeningSocket(int fd) const
{
  return listeningSockets.find(fd) != listeningSockets.end();
}

void ServerManager::setIsRunning(bool value)
{
  this->isRunning = value;
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
  if (!eventReporter.addFD(clientFd))
  {
    std::cerr << "Error: Failed to add client socket to EventReporter" << std::endl;
    return;
  }

  Client* client = new Client(listenFd, clientFd, ports[listenFd]);
  clients[clientFd] = client;
}

void ServerManager::processClientInput(int clientFd)
{
  if (clients.find(clientFd) == clients.end())
  {
    std::cerr << "Warning: Data sent to non-existing client" << std::endl;
    return;
  }
  bool status = clients[clientFd]->processInput();
  if (!status)
  {
    closeConnection(clientFd);
  }
}

void ServerManager::closeConnection(int clientFd)
{
  if (clients.find(clientFd) != clients.end())
  {
    clients[clientFd]->closeConnection();
    delete clients[clientFd];
    clients.erase(clientFd);
  }
}