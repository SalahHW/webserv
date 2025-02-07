#include "VirtualHost.hpp"

VirtualHost::~VirtualHost() { }

VirtualHost::VirtualHost()
    : clientMaxBodySize(-1)
    , clientTimeOut(0)
{
}

VirtualHost::VirtualHost(const VirtualHost& other) { *this = other; }

VirtualHost& VirtualHost::operator=(const VirtualHost& other)
{
  if (this != &other)
  {
    this->name = other.name;
    this->clientBodyTempPath = other.clientBodyTempPath;
    this->clientMaxBodySize = other.clientMaxBodySize;
    this->clientTimeOut = other.clientTimeOut;
    this->errorPages = other.errorPages;
    this->locations = other.locations;
  }
  return *this;
}

void VirtualHost::setName(const std::string& name)
{
  this->name = name;
}

void VirtualHost::setClientBodyTempPath(const std::string& path)
{
  this->clientBodyTempPath = path;
}

void VirtualHost::setClientMaxBodySize(int clientMaxBodySize)
{
  this->clientMaxBodySize = clientMaxBodySize;
}

void VirtualHost::setClientTimeOut(int timeOut)
{
  this->clientTimeOut = timeOut;
}

void VirtualHost::setErrorPages(
    const std::map<size_t, std::string>& errorPages)
{
  this->errorPages = errorPages;
}

void VirtualHost::setLocations(
    const std::map<std::string, Location>& locations)
{
  this->locations = locations;
}

const std::string& VirtualHost::getName() const
{
  return name;
}

const std::string& VirtualHost::getClientBodyTempPath() const
{
  return clientBodyTempPath;
}

int VirtualHost::getClientMaxBodySize() const
{
  return clientMaxBodySize;
}

int VirtualHost::getClientTimeOut() const
{
  return clientTimeOut;
}

const std::map<size_t, std::string>& VirtualHost::getErrorPages() const
{
  return errorPages;
}

const std::map<std::string, Location>& VirtualHost::getLocations() const
{
  return locations;
}