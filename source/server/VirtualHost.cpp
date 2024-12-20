#include "VirtualHost.h"

VirtualHost::VirtualHost() {
}

VirtualHost::~VirtualHost() {
}

VirtualHost::VirtualHost(const VirtualHost& other) {
    this->clientMaxBodySize = other.clientMaxBodySize;
    this->errorPages = other.errorPages;
    this->locations = other.locations;
}

VirtualHost& VirtualHost::operator=(const VirtualHost& other) {
    this->clientMaxBodySize = other.clientMaxBodySize;
    this->errorPages = other.errorPages;
    this->locations = other.locations;
    return *this;
}

void VirtualHost::setClientMaxBodySize(const std::string& clientMaxBodySize) {
    this->clientMaxBodySize = clientMaxBodySize;
}

void VirtualHost::setErrorPages(const std::map<int, std::string>& errorPages) {
    this->errorPages = errorPages;
}

void VirtualHost::setLocations(const std::map<std::string, Location>& locations) {
    this->locations = locations;
}

const std::string& VirtualHost::getClientMaxBodySize() const {
    return clientMaxBodySize;
}

const std::map<int, std::string>& VirtualHost::getErrorPages() const {
    return errorPages;
}

const std::map<std::string, Location>& VirtualHost::getLocations() const {
    return locations;
}