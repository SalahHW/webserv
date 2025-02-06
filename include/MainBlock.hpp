#pragma once

#include "Block.hpp"

class MainBlock : public Block {
 public:
  virtual ~MainBlock();
  MainBlock();
  MainBlock(const MainBlock& src);
  MainBlock& operator=(const MainBlock& src);

  bool validateSpecific();

 private:
};