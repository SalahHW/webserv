#include "ServerBlock.hpp"

ServerBlock::~ServerBlock() {}

ServerBlock::ServerBlock(const std::string &fullBlockLine, Block *contextBlock)
    : Block(fullBlockLine, contextBlock) {
  addValidContext("main");
  addValidContext("http");
  setMinArgs(0);
  setMaxArgs(0);
}

ServerBlock::ServerBlock(const ServerBlock &other) : Block(other) {}

ServerBlock &ServerBlock::operator=(const ServerBlock &other) {
  if (this != &other) {
    Block::operator=(other);
  }
  return *this;
}

bool ServerBlock::validateSpecific() {
  // Placeholder implementation, update as needed
  return true;
}
