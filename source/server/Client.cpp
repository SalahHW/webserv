#include "Client.hpp"

#include <cstdlib>
#include <sstream>
#include <string>

#include "Sender.hpp"

Client::~Client()
{
  std::cout << RED << "Client fd " << connectionFd << " connection closed"
            << RESET << std::endl;
  if (request)
  {
    delete request;
  }
}

Client::Client(int listenFd, int connectionFd, Port* port)
    : listenFd(listenFd)
    , connectionFd(connectionFd)
    , associatedPort(port)
    , request(NULL)
{
  initEv();
  lastActivity = getCurrentTime();
  std::cout << GREEN << "New client connected on fd: " RESET << connectionFd
            << GREEN " port: " RESET << port->getPort() << std::endl;
}

void Client::initEv()
{
  ev.events = EPOLLIN;
  ev.data.fd = connectionFd;
}

std::string& Client::getBuffer() { return buffer; }

int Client::getListenFd() const { return this->listenFd; }

int Client::getConnectionFd() const { return this->connectionFd; }

void Client::closeConnection()
{
  close(connectionFd);
  clearBuffer();
}

void Client::appendToBuffer(const char* data, size_t len)
{
  buffer.append(data, len);
}

int Client::getEvent() const { return ev.events; }

void Client::clearBuffer() { buffer.clear(); }

int Client::readFromClient()
{
  char buffer[1024];

  ssize_t bytesRead = recv(connectionFd, buffer, sizeof(buffer) - 1, 0);
  if (bytesRead <= 0)
  {
    if (bytesRead < 0)
    {
      std::cerr << "Read error on client fd " << connectionFd << std::endl;
    }
    if (request)
    {
      request->setIsTreated(true);
    }
    return bytesRead;
  }
  buffer[bytesRead] = '\0';
  appendToBuffer(buffer, bytesRead);
  if (bytesRead > 0)
  {
    this->lastActivity = getCurrentTime();
  }
  return bytesRead;
}

void Client::eventToOut()
{
  ev.events = EPOLLOUT;
  epoll_ctl(epollFd, EPOLL_CTL_MOD, connectionFd, &ev);
}

void Client::eventToIn()
{
  ev.events = EPOLLIN;
  epoll_ctl(epollFd, EPOLL_CTL_MOD, connectionFd, &ev);
}

void Client::eventToErr()
{
  ev.events = EPOLLERR;
  epoll_ctl(epollFd, EPOLL_CTL_MOD, connectionFd, &ev);
}

double Client::getCurrentTime(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)(tv.tv_sec) + (double)(tv.tv_usec) / 1e6;
}

void Client::treatAPost()
{
  request = new Request(getBuffer(), connectionFd);
  clearBuffer();
}

size_t Client::parseContentLength(const std::string& headers)
{
  const std::string contentLengthKey = "Content-Length:";
  std::string::size_type pos = headers.find(contentLengthKey);
  if (pos == std::string::npos)
  {
    return 0;
  }
  pos += contentLengthKey.size();
  while (pos < headers.size() && (headers[pos] == ' ' || headers[pos] == '\t'))
  {
    ++pos;
  }
  std::string numberStr;
  while (pos < headers.size() && std::isdigit(headers[pos]))
  {
    numberStr.push_back(headers[pos]);
    ++pos;
  }
  if (numberStr.empty())
  {
    return 0;
  }
  size_t result = 0;
  for (std::string::size_type i = 0; i < numberStr.size(); ++i)
  {
    result = result * 10 + (numberStr[i] - '0');
  }
  return result;
}

void Client::requestRoutine()
{
  if (readFromClient() <= 0)
  {
    eventToErr();
    return;
  }
  if (buffer.find("POST") != std::string::npos)
  {
    treatAPost();
    eventToOut();
  }
  else if (buffer.find("\r\n\r\n") != std::string::npos)
  {
    eventToOut();
    request = new Request(getBuffer(), connectionFd);
    clearBuffer();
  }
}

std::string Client::removeFinalBoundary(const std::string& input)
{
  static const std::string boundary = "-----------------------------";
  static const std::string endPattern = "--\r\n";

  if (input.size() < endPattern.size())
  {
    return input;
  }

  if (input.compare(input.size() - endPattern.size(), endPattern.size(),
          endPattern)
      != 0)
  {
    return input;
  }
  request->setIsTreated(true);
  std::string::size_type pos = input.size() - endPattern.size();
  while (pos > 0 && std::isdigit(static_cast<unsigned char>(input[pos - 1])))
  {
    --pos;
  }

  if (pos >= boundary.size())
  {
    if (input.compare(pos - boundary.size(), boundary.size(), boundary) == 0)
    {
      std::string::size_type startToRemove = pos - boundary.size();
      return input.substr(0, startToRemove);
    }
  }

  return input;
}

void Client::handleGet()
{
  if (request->getIsTreated())
  {
    eventToErr();
    return;
  }
  if (request->getIsParsed())
  {
    if (request->getIsInTreatment())
    {
      request->getResponse()->getResponseBuilder()->buildBody();
    }
    else
    {
      request->setIsInTreatment(true);
      request->setResponse(associatedPort->getVirtualHosts(),
          associatedPort->getDefaultVirtualHostName());
    }
    Sender sender(*request->getResponse(), connectionFd, *request, this);
    this->lastActivity = getCurrentTime();
  }
}

void Client::handlePost()
{
  if (request->getIsParsed())
  {
    if (request->getIsInTreatment())
    {
      if (!request->getIsTreated() && readFromClient() > 0)
      {
        buffer = removeFinalBoundary(buffer);
        request->setFileContent(getBuffer());
        request->getResponse()->getResponseBuilder()->treatAPost();
        clearBuffer();
      }
      if (request->getIsTreated())
      {
        request->getResponse()->getResponseBuilder()->successPost();
        eventToErr();
      }
    }
    else
    {
      request->setIsInTreatment(false);
      request->setResponse(associatedPort->getVirtualHosts(),
          associatedPort->getDefaultVirtualHostName());
      request->setIsInTreatment(true);
    }
  }
  if (request->getIsTreated())
  {
    Sender sender(*request->getResponse(), connectionFd, *request, this);
    this->lastActivity = getCurrentTime();
    eventToErr();
  }
  this->lastActivity = getCurrentTime();
  return;
}

void Client::handleDelete()
{
  request->setIsInTreatment(true);
  request->setResponse(associatedPort->getVirtualHosts(),
      associatedPort->getDefaultVirtualHostName());
  if (request->getIsTreated())
  {
    request->getResponse()->getResponseBuilder()->successPost();
    Sender sender(*request->getResponse(), connectionFd, *request, this);
    eventToErr();
  }
  this->lastActivity = getCurrentTime();
  return;
}

void Client::handleCgi()
{
  request->setIsACgi(true);
  if (request->getIsInTreatment())
  {
    request->getResponse()->getResponseBuilder()->buildBody();
  }
  else
  {
    request->setIsInTreatment(true);
    request->setResponse(associatedPort->getVirtualHosts(),
        associatedPort->getDefaultVirtualHostName());
  }
  this->lastActivity = getCurrentTime();
  return;
}

void Client::responsesRoutine()
{

  if (request->getIsTreated())
  {
    eventToErr();
    return;
  }
  else if (request->getUri().find("cgi-bin") != std::string::npos)
  {
    handleCgi();
    if (!request->getResponse()->getBody().empty())
    {
      Sender sender(*request->getResponse(), connectionFd, *request, this);
    }
    else
    {

      request->setIsTreated(true);
    }
    return;
  }
  else if (request->getMethod() == "POST")
  {
    handlePost();
    return;
  }
  else if (request->getMethod() == "DELETE")
  {
    handleDelete();
    eventToErr();
    return;
  }
  handleGet();
  if (request->getIsTreated())
  {
    eventToErr();
    return;
  }
}
