#include "VirtualHost.hpp"

VirtualHost::~VirtualHost() {}

VirtualHost::VirtualHost() : name(""), clientMaxBodySize(-1) {}

VirtualHost::VirtualHost(const VirtualHost& other) { *this = other; }

VirtualHost& VirtualHost::operator=(const VirtualHost& other) {
  if (this != &other) {
    this->name = other.name;
    this->clientMaxBodySize = other.clientMaxBodySize;
    this->errorPages = other.errorPages;
    this->locations = other.locations;
  }
  return *this;
}

void VirtualHost::setName(const std::string& name) {
  this->name = name;
  std::cout << "VirtualHost::setName: " << name << std::endl;
}

void VirtualHost::setClientMaxBodySize(int clientMaxBodySize) {
  this->clientMaxBodySize = clientMaxBodySize;
}

void VirtualHost::setErrorPages(
    const std::map<size_t, std::string>& errorPages) {
  this->errorPages = errorPages;
}

void VirtualHost::setLocations(
    const std::map<std::string, Location>& locations) {
  this->locations = locations;
}

const std::string& VirtualHost::getName() const { return name; }

int VirtualHost::getClientMaxBodySize() const { return clientMaxBodySize; }

const std::map<size_t, std::string>& VirtualHost::getErrorPages() const {
  return errorPages;
}

const std::map<std::string, Location>& VirtualHost::getLocations() const {
  return locations;
}