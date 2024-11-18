#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "Directive.hpp"

class Block {
 public:
  virtual ~Block();
  Block(const std::string& fullBlockLine, Block* contextBlock);
  Block(const Block& other);
  Block& operator=(const Block& other);

  void validate();
  bool validateContext() const;
  bool validateArgsSize() const;

  virtual bool validateSpecific() = 0;

  std::string getName() const;
  Block* getContextBlock() const;
  std::string getFullBlockLine() const;
  std::vector<Block*> getSubBlocks() const;
  std::vector<std::string> const& getArguments() const;
  std::vector<Directive*> getDirectives() const;
  bool good() const;

  void addSubBlock(Block* block);
  void addDirective(Directive* directive);
  void addValidContext(const std::string& context);

  void printBlock(int indent = 0) const;

  virtual void apply(Location& location) const;

 protected:
  bool isValid;
  std::string fullLine;
  Block* contextBlock;
  std::string name;
  std::vector<std::string> arguments;
  std::vector<Block*> subBlocks;
  std::vector<Directive*> directives;
  std::vector<std::string> validContexts;
  int minArgs;
  int maxArgs;

 private:
  Block();

  void tokenizeName();
};
