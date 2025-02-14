#include "LocationBlock.hpp"

#include "utils.hpp"

LocationBlock::~LocationBlock() {}

LocationBlock::LocationBlock(const std::string &fullBlockLine,
                             Block *contextBlock)
    : Block(fullBlockLine, contextBlock) {
  addValidContext("server");
  addValidContext("location");
  setMinArgs(1);
  setMaxArgs(2);
  validate();
}

LocationBlock::LocationBlock(const LocationBlock &other) : Block(other) {}

LocationBlock &LocationBlock::operator=(const LocationBlock &other) {
  if (this != &other) {
    Block::operator=(other);
  }
  return *this;
}

bool LocationBlock::validateSpecific() {
  std::vector<std::string> arguments = getArguments();
  std::set<std::string> uniquePaths(arguments.begin(), arguments.end());
  if (uniquePaths.size() != arguments.size()) {
    std::cerr << "Error: Duplicate paths found in location block." << std::endl;
    return false;
  }
  return true;
}

void LocationBlock::apply(Location &location) const {
  std::vector<std::string> arguments = getArguments();
  for (size_t i = 0; i < arguments.size(); ++i) {
    location.addPath(arguments[i]);
  }
  location.setPath(arguments[arguments.size() - 1]);
}
