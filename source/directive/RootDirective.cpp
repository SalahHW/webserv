#include "Directive.hpp"

RootDirective::~RootDirective() { }

RootDirective::RootDirective(Block* currentContext,
    const std::string& fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine)
{
  setName("root");
  setMinArgs(1);
  setMaxArgs(1);
  addContext("location");
  validate();
}

RootDirective::RootDirective(const RootDirective& other)
    : Directive(other)
    , rootPath(other.rootPath)
{
}

RootDirective& RootDirective::operator=(const RootDirective& other)
{
  if (this != &other)
  {
    Directive::operator=(other);
    rootPath = other.rootPath;
  }
  return *this;
}

bool RootDirective::validateSpecific()
{
  rootPath = getArguments()[0];
  // TODO: Implement validation
  return true;
}

void RootDirective::apply(Location& location)
{
  location.setRootDirectory(rootPath);
}

void RootDirective::setRootPath(const std::string& rootPath)
{
  this->rootPath = rootPath;
}

const std::string& RootDirective::getRootPath() const { return rootPath; }