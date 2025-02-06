#include "Directive.hpp"

ErrorPageDirective::~ErrorPageDirective() { }

ErrorPageDirective::ErrorPageDirective(Block* currentContext, const std::string& fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine)
{
  setName("error_page");
  setMinArgs(2);
  setMaxArgs(2);
  addContext("server");
  validate();
}

ErrorPageDirective::ErrorPageDirective(const ErrorPageDirective& other)
    : Directive(other)
    , errorCode(other.errorCode)
    , errorPath(other.errorPath)
{
}

ErrorPageDirective& ErrorPageDirective::operator=(const ErrorPageDirective& other)
{
  if (this != &other)
  {
    Directive::operator=(other);
    errorCode = other.errorCode;
    errorPath = other.errorPath;
  }
  return *this;
}

bool ErrorPageDirective::validateSpecific()
{
  if (!utils::convertToInt(getArguments()[0].c_str(), errorCode))
  {
    std::cerr << "Error: Directive \"" << getName() << "\" has an invalid argument." << std::endl;
    return false;
  }
  errorPath = getArguments()[1];
  return true;
}

void ErrorPageDirective::apply(Server& server)
{
  server.addErrorPage(errorCode, errorPath);
}

void ErrorPageDirective::setErrorPage(const std::string& errorPage)
{
  // TODO: Implement setErrorPage
  (void)errorPage;
}

std::string ErrorPageDirective::getErrorPage() const
{
  // TODO: Implement getErrorPage
  return "";
}