#include "Block.hpp"

Block::~Block() {
  for (size_t i = 0; i < subBlocks.size(); ++i) {
    delete subBlocks[i];
  }
  for (size_t i = 0; i < directives.size(); ++i) {
    delete directives[i];
  }
}

Block::Block(const std::string &fullBlockLine, Block *contextBlock)
    : isValid(true), fullLine(fullBlockLine), contextBlock(contextBlock) {
  tokenizeName();
}

Block::Block(const Block &other)
    : isValid(other.isValid),
      fullLine(other.fullLine),
      contextBlock(other.contextBlock),
      name(other.name),
      arguments(other.arguments),
      subBlocks(other.subBlocks),
      directives(other.directives),
      validContexts(other.validContexts),
      minArgs(other.minArgs),
      maxArgs(other.maxArgs) {}

Block &Block::operator=(const Block &other) {
  if (this != &other) {
    isValid = other.isValid;
    fullLine = other.fullLine;
    contextBlock = other.contextBlock;
    name = other.name;
    arguments = other.arguments;
    subBlocks = other.subBlocks;
    directives = other.directives;
    validContexts = other.validContexts;
    minArgs = other.minArgs;
    maxArgs = other.maxArgs;
  }
  return *this;
}

bool Block::validateContext() const {
  if (std::find(validContexts.begin(), validContexts.end(),
                contextBlock->getName()) == validContexts.end()) {
    std::cerr << "Error: Block \"" << name << "\" cannot be used in \""
              << contextBlock->getName() << "\" block" << std::endl;
    return false;
  }
  return true;
}

bool Block::validateArgsSize() const {
  int argsSize = static_cast<int>(arguments.size());

  if (argsSize < minArgs || argsSize > maxArgs) {
    std::cerr << "Error: Block \"" << fullLine
              << "\" has an invalid number of arguments." << std::endl;
    return false;
  }
  return true;
}

void Block::validate() {
  if (name == "main")
    // TODO: Check if we should also validate context or validate specific;
    isValid = validateSpecific();
  else
    isValid = validateContext() && validateArgsSize() && validateSpecific();
}

bool Block::getIsValid() const { return isValid; }
const std::string &Block::getFullBlockLine() const { return fullLine; }
const Block *Block::getContextBlock() const { return contextBlock; }
const std::string &Block::getName() const { return name; }
const std::vector<std::string> &Block::getArguments() const {
  return arguments;
}
const std::vector<Block *> &Block::getSubBlocks() const { return subBlocks; }
const std::vector<Directive *> &Block::getDirectives() const { return directives; }
int Block::getMinArgs() const { return minArgs; }
int Block::getMaxArgs() const { return maxArgs; }


bool Block::good() const { return isValid; }

void Block::setIsValid(bool isValid) { this->isValid = isValid; }
void Block::setName(const std::string &name) { this->name = name; }
void Block::setMinArgs(int min) { minArgs = min; }
void Block::setMaxArgs(int max) { maxArgs = max; }

void Block::addSubBlock(Block *block) { subBlocks.push_back(block); }

void Block::addDirective(Directive *directive) {
  directives.push_back(directive);
}

void Block::addValidContext(const std::string &context) {
  validContexts.push_back(context);
}

void Block::printBlock(int indent) const {
  for (int i = 0; i < indent; ++i) {
    std::cout << "  ";
  }
  std::cout << "Block: " << name << std::endl;

  for (size_t i = 0; i < directives.size(); ++i) {
    for (int j = 0; j < indent + 1; ++j) {
      std::cout << "  ";
    }
    std::cout << "Directive: " << directives[i]->getFullDirectiveLine()
              << std::endl;
  }

  for (size_t i = 0; i < subBlocks.size(); ++i) {
    subBlocks[i]->printBlock(indent + 1);
  }
}

void Block::tokenizeName() {
  std::stringstream ss(fullLine);
  std::string token;

  ss >> name;

  while (ss >> token) {
    arguments.push_back(token);
  }
}

void Block::apply(Location &location) const { (void)location; }
