#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "Block.hpp"
#include "BlockFactory.hpp"
#include "DirectiveFactory.hpp"
#include "utils.hpp"

class ConfigFile {
 public:
  // Constructor and destructor
  ConfigFile(const std::string& configFilePath);
  ~ConfigFile();

  // Accessors
  const Block& getMainBlock() const;
  bool good() const;

 private:
  // Disable default constructor, copy constructor, and assignment operator
  ConfigFile();
  ConfigFile(const ConfigFile& other);
  ConfigFile& operator=(const ConfigFile& other);

  // Parsing methods
  void readConfigFile(const std::string& fileName);
  void parseFile(std::ifstream& file, std::stack<Block*>& blockStack);
  bool lineEndsProperly(const std::string& line) const;
  void handleLine(const std::string& cleanedLine, std::ifstream& file,
                  std::stack<Block*>& blockStack);
  void processBlockDeclaration(const std::string& line, std::ifstream& file,
                               std::stack<Block*>& blockStack,
                               Block* currentBlock);
  void parseBlock(std::ifstream& file, std::stack<Block*>& blockStack);
  void processDirective(const std::string& cleanedLine, Block* currentBlock);

  // Utility methods
  bool isOpeningBracePresent(std::size_t openBracePos,
                             std::size_t semiColonPos) const;
  bool findOpeningBrace(std::ifstream& file, Block* currentBlock);
  std::string cleanLine(const std::string& originalLine) const;
  bool isDirective(const std::string& line) const;
  std::string extractBlockName(const std::string& line) const;
  std::string extractDirectiveName(const std::string& line) const;

  // Member variables
  DirectiveFactory directiveFactory;
  BlockFactory blockFactory;
  Block* mainBlock;
  bool isValid;
};