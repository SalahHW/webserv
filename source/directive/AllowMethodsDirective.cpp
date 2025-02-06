#include "Directive.hpp"
#include "Block.hpp"

AllowMethodsDirective::~AllowMethodsDirective() { }

AllowMethodsDirective::AllowMethodsDirective(Block* currentContext, const std::string& fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine)
    , getAllowed(false)
    , postAllowed(false)
    , deleteAllowed(false)
{
  setName("allow_methods");
  setMinArgs(1);
  setMaxArgs(3);
  addContext("location");
  validate();
}

AllowMethodsDirective::AllowMethodsDirective(const AllowMethodsDirective& other)
    : Directive(other)
{
}

AllowMethodsDirective& AllowMethodsDirective::operator=(const AllowMethodsDirective& other)
{
  if (this != &other)
  {
    this->getAllowed = other.getAllowed;
    this->postAllowed = other.postAllowed;
    this->deleteAllowed = other.deleteAllowed;
  }
  return *this;
}

bool AllowMethodsDirective::validateSpecific()
{
  const std::vector<std::string> arguments = getArguments();
  for (size_t i = 0; i < arguments.size(); i++)
  {
    std::string method = arguments[i];
    utils::toLowerCase(method);
    if (!isValidMethod(method))
    {
      std::cerr << "Error: Bad argument in " << this->getName() << " directive inside \""
                << getCurrentContext()->getName() << "\" block." << std::endl;
      return false;
    }
    processMethod(method);
  }
  return true;
}

bool AllowMethodsDirective::isValidMethod(const std::string& method) const
{
  if (method == "get" || method == "post" || method == "delete")
  {
    return true;
  }
  return false;
}

void AllowMethodsDirective::processMethod(const std::string& method)
{
  if (method == "get")
  {
    this->getAllowed = true;
  }
  else if (method == "post")
  {
    this->postAllowed = true;
  }
  else if (method == "delete")
  {
    this->deleteAllowed = true;
  }
}

void AllowMethodsDirective::apply(Location& location)
{
  location.setGetAccepted(this->getAllowed);
  location.setPostAccepted(this->postAllowed);
  location.setDeleteAccepted(this->deleteAllowed);
}