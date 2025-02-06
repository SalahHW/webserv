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

  bool getIsValid() const;
  const std::string& getFullBlockLine() const;
  const Block* getContextBlock() const;
  const std::string& getName() const;
  const std::vector<std::string>& getArguments() const;
  const std::vector<Block*>& getSubBlocks() const;
  const std::vector<Directive*>& getDirectives() const;
  const std::vector<std::string>& getValidContexts() const;
  int getMinArgs() const;
  int getMaxArgs() const;

  bool good() const;

  void setIsValid(bool isValid);
  void setName(const std::string& name);
  void setMinArgs(int min);
  void setMaxArgs(int max);

  void addSubBlock(Block* block);
  void addDirective(Directive* directive);
  void addValidContext(const std::string& context);

  void printBlock(int indent = 0) const;

  virtual void apply(Location& location) const;

 private:
  Block();

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

  void tokenizeName();
};
