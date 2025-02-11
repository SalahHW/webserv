#include "Client.hpp"

Client::~Client()
{
  std::cout << RED << "Client fd " << connectionFd << " connection closed" << RESET << std::endl;
}

Client::Client(int listenFd, int connectionFd, Port* port)
    : listenFd(listenFd)
    , connectionFd(connectionFd)
    , associatedPort(port)
{
  std::cout << GREEN << "New client connected on fd: " RESET << connectionFd << GREEN " port: " RESET
            << port->getPort() << std::endl;
}

std::string& Client::getBuffer()
{
  return buffer;
}

int Client::getListenFd() const
{
  return this->listenFd;
}

int Client::getConnectionFd() const
{
  return this->connectionFd;
}

void Client::closeConnection()
{
  close(connectionFd);
  clearBuffer();
}

void Client::appendToBuffer(const char* data, size_t len)
{
  buffer.append(data, len);
}

void Client::clearBuffer()
{
  buffer.clear();
}

bool Client::processInput()
{
  char recvBuffer[1024];
  ssize_t bytesRead = recv(connectionFd, recvBuffer, sizeof(recvBuffer) - 1, 0);

  if (bytesRead <= 0)
  {
    if (bytesRead < 0)
    {
      std::cerr << "Error reading from client fd: " << connectionFd << std::endl;
    }
    return false;
  }

  recvBuffer[bytesRead] = '\0';
  appendToBuffer(recvBuffer, bytesRead);

  // Parsing the request
  Request request(buffer);
  // request.displayRequest();
  std::string response = RequestRouter::route(request);

  send(connectionFd, response.c_str(), response.size(), 0);

  clearBuffer();
  return true;
}