#include "MainBlock.hpp"

MainBlock::~MainBlock() {}

MainBlock::MainBlock() : Block("main", NULL) {
  setMinArgs(0);
  setMaxArgs(0);
  validate();
}

MainBlock::MainBlock(const MainBlock &other) : Block(other) {}

MainBlock &MainBlock::operator=(const MainBlock &other) {
  if (this != &other) {
    Block::operator=(other);
  }
  return *this;
}

bool MainBlock::validateSpecific() { return true; }
