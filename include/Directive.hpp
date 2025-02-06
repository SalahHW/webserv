#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Location.hpp"
#include "Server.hpp"

class Block;

class Directive {
 public:
  virtual ~Directive();
  Directive(Block* currentContext, const std::string& fullDirectiveLine);
  Directive(const Directive& other);
  Directive& operator=(const Directive& other);

  void validate();
  bool validateContext() const;
  bool validateArgsSize() const;

  virtual bool validateSpecific() = 0;
  virtual void displayInfo() const = 0;
  virtual void apply(Server& server);
  virtual void apply(Location& location);

  bool getIsValid() const { return isValid; };
  const Block* getCurrentContext() const;
  const std::string &getFullDirectiveLine() const;
  const std::string &getName() const;
  const std::vector<std::string> &getArguments() const;
  int getMinArgs() const;
  int getMaxArgs() const;
  const std::vector<std::string> &getContexts() const;
  bool good() const;

  void setIsValid(bool valid);
  void setFullDirectiveLine(const std::string& line);
  void setName(const std::string& name);
  void setCurrentContext(Block* context);
  void setMinArgs(int min);
  void setMaxArgs(int max);

  void addArgument(const std::string& argument);
  void addContext(const std::string& context);

 private:
  Directive();

  bool isValid;
  Block* currentContext;
  std::string fullDirectiveLine;
  std::string name;
  std::vector<std::string> arguments;
  int minArgs;
  int maxArgs;
  std::vector<std::string> contexts;

  void tokenizeName();
};
