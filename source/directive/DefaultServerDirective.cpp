#include "Directive.hpp"

DefaultServerDirective::~DefaultServerDirective() { }

DefaultServerDirective::DefaultServerDirective(
    Block* currentContext, const std::string& fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine)
{
  setName("default_server");
  setMinArgs(0);
  setMaxArgs(0);
  addContext("server");
  validate();
}

DefaultServerDirective::DefaultServerDirective(const DefaultServerDirective& other)
    : Directive(other)
{
}

DefaultServerDirective& DefaultServerDirective::operator=(const DefaultServerDirective& other)
{
  if (this != &other)
  {
    Directive::operator=(other);
  }
  return *this;
}

bool DefaultServerDirective::validateSpecific() { return true; }

void DefaultServerDirective::apply(Server& server)
{
  server.setExplicitlyDefault(true);
}