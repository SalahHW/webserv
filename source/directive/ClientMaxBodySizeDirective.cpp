#include "ClientMaxBodySizeDirective.hpp"

ClientMaxBodySizeDirective::~ClientMaxBodySizeDirective() {}

ClientMaxBodySizeDirective::ClientMaxBodySizeDirective(
    Block *currentContext, const std::string &fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine), maxBodySize(0) {
  setName("client_max_body_size");
  setMinArgs(1);
  setMaxArgs(1);
  addContext("server");
  addContext("location");
  validate();
}

ClientMaxBodySizeDirective::ClientMaxBodySizeDirective(
    const ClientMaxBodySizeDirective &other)
    : Directive(other), maxBodySize(other.maxBodySize) {}

ClientMaxBodySizeDirective &ClientMaxBodySizeDirective::operator=(
    const ClientMaxBodySizeDirective &other) {
  if (this != &other) {
    Directive::operator=(other);
    maxBodySize = other.maxBodySize;
  }
  return *this;
}

bool ClientMaxBodySizeDirective::validateSpecific() {
  std::vector<std::string> arguments = getArguments();
  char lastChar = arguments[0][arguments[0].size() - 1];
  long multiplier;
  if (isdigit(lastChar)) {
    multiplier = 1;
  } else if (lastChar == 'k' || lastChar == 'K') {
    arguments[0].erase(arguments[0].size() - 1);
    multiplier = 1024;
  } else if (lastChar == 'm' || lastChar == 'M') {
    arguments[0].erase(arguments[0].size() - 1);
    multiplier = 1024 * 1024;
  } else if (lastChar == 'g' || lastChar == 'G') {
    arguments[0].erase(arguments[0].size() - 1);
    multiplier = 1024 * 1024 * 1024;
  } else {
    std::cerr << "Error: Directive \"" << getName()
              << "\" has an invalid argument." << std::endl;
    return false;
  }

  if (!utils::convertToInt(arguments[0].c_str(), maxBodySize)) {
    std::cerr << "Error: Directive \"" << getName()
              << "\" has an invalid argument." << std::endl;
    return false;
  }
  if (!utils::safeMultiplyInt(maxBodySize, multiplier, maxBodySize)) {
    std::cerr << "Error: Directive \"" << getName()
              << "\" exceed the maximum size." << std::endl;
    return false;
  }
  return true;
}

void ClientMaxBodySizeDirective::displayInfo() const {
  std::cout << "Name : " << this->getName() << std::endl
            << "- max body size : " << maxBodySize << std::endl;
}

void ClientMaxBodySizeDirective::apply(Server &server) {
  server.setClientMaxBodySize(maxBodySize);
}

void ClientMaxBodySizeDirective::apply(Location &location) {
  location.setClientMaxBodySize(maxBodySize);
}

void ClientMaxBodySizeDirective::setMaxBodySize(size_t maxBodySize) {
  this->maxBodySize = maxBodySize;
}

size_t ClientMaxBodySizeDirective::getMaxBodySize() const {
  return maxBodySize;
}