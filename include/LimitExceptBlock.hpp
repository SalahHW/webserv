#pragma once

#include <map>

#include "Block.hpp"

class LimitExceptBlock : public Block {
 public:
  virtual ~LimitExceptBlock();
  LimitExceptBlock(const std::string& fullBlockLine, Block* contextBlock);
  LimitExceptBlock(const LimitExceptBlock& other);
  LimitExceptBlock& operator=(const LimitExceptBlock& other);

  virtual bool validateSpecific();

 protected:
  bool isGetConcerned;
  bool isPostConcerned;
  bool isDeleteConcerned;

 private:
  bool extractArguments();
};