#include "Server.hpp"

#include "Location.hpp"
#include "ServerHandler.hpp"

Server::~Server() { }

Server::Server()
    : port(-1)
    , name("")
    , clientBodyTempPath("")
    , clientMaxBodySize(-1)
    , explicitlyDefault(false)
    , hasPort(false)
    , hasName(false)
    , hasClientBodyTempPath(false)
    , hasClientMaxBodySize(false)
    , hasErrorPages(false)
    , hasLocations(false)
{
}

Server::Server(const Server& src) { *this = src; }

Server& Server::operator=(const Server& src)
{
  if (this != &src)
  {
    this->port = src.port;
    this->name = src.name;
    this->clientBodyTempPath = src.clientBodyTempPath;
    this->clientMaxBodySize = src.clientMaxBodySize;
    this->isDefault = src.isDefault;
    this->explicitlyDefault = src.explicitlyDefault;
    this->errorPages = src.errorPages;
    this->locations = src.locations;
    this->hasPort = src.hasPort;
    this->hasName = src.hasName;
    this->hasClientBodyTempPath = src.hasClientBodyTempPath;
    this->hasClientMaxBodySize = src.hasClientMaxBodySize;
    this->hasErrorPages = src.hasErrorPages;
    this->hasLocations = src.hasLocations;
  }
  return *this;
}

void Server::setPort(int port)
{
  this->port = port;
  this->hasPort = true;
}

void Server::setClientMaxBodySize(int size)
{
  this->clientMaxBodySize = size;
  this->hasClientMaxBodySize = true;
}

void Server::setName(const std::string& name)
{
  this->name = name;
  this->hasName = true;
}

void Server::setClientBodyTempPath(const std::string& clientBodyTempPath)
{
  this->clientBodyTempPath = clientBodyTempPath;
  this->hasClientBodyTempPath = true;
}

void Server::setErrorPages(std::map<size_t, std::string> errorPages)
{
  this->errorPages = errorPages;
  this->hasErrorPages = true;
}

void Server::setExplicitlyDefault(bool isExplicitlyDefault)
{
  this->explicitlyDefault = isExplicitlyDefault;
}

void Server::addLocation(const Location& location)
{
  this->locations[location.getPath()] = location;
  this->hasLocations = true;
}

void Server::addErrorPage(int errorCode, std::string errorPath)
{
  errorPages[errorCode] = errorPath;
  this->hasErrorPages = true;
}

int Server::getPort() const
{
  return this->port;
}

int Server::getClientMaxBodySize() const
{
  return this->clientMaxBodySize;
}

const std::string& Server::getName() const
{
  return this->name;
}

const std::string& Server::getClientBodyTempPath() const
{
  return this->clientBodyTempPath;
}

const std::map<size_t, std::string>& Server::getErrorPages() const
{
  return this->errorPages;
}

std::map<std::string, Location>& Server::getLocations()
{
  return this->locations;
}

const std::map<std::string, Location>& Server::getLocations() const
{
  return this->locations;
}

bool Server::isExplicitlyDefault() const
{
  return this->explicitlyDefault;
}

void Server::displayServerInfo() const
{
  std::cout << "Server \"" << name << "\" Informations:" << std::endl;
  if (this->hasPort)
    std::cout << "- Port: " << port << std::endl;
  if (this->hasName)
    std::cout << "- Name: " << name << std::endl;
  if (this->hasClientBodyTempPath)
    std::cout << "- Client Body Temp Path: " << clientBodyTempPath << std::endl;
  if (this->hasClientMaxBodySize)
    std::cout << "- Client Max Body Size: " << clientMaxBodySize << " bytes" << std::endl;
  std::cout << "- Default Server: " << (isDefault ? "true" : "false") << std::endl;
  std::cout << "- Explicitly Default: " << (explicitlyDefault ? "true" : "false") << std::endl;
  if (this->hasErrorPages)
  {
    std::map<size_t, std::string>::const_iterator errorPages_it = errorPages.begin();
    std::cout << "- Error Pages: " << std::endl;
    while (errorPages_it != errorPages.end())
    {
      std::cout << "  * " << errorPages_it->first << " "
                << errorPages_it->second << std::endl;
      errorPages_it++;
    }
  }
  if (this->hasLocations)
  {
    std::map<std::string, Location>::const_iterator locations_it = locations.begin();
    std::cout << "- Locations: " << std::endl;
    while (locations_it != locations.end())
    {
      locations_it->second.displayLocationInfo();
      locations_it++;
    }
  }
}