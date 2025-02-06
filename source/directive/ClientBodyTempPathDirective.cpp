#include "ClientBodyTempPathDirectice.hpp"

ClientBodyTempPathDirective::~ClientBodyTempPathDirective() { }

ClientBodyTempPathDirective::ClientBodyTempPathDirective(Block* currentContext, const std::string& fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine)
{
  setName("client_body_temp_path");
  setMinArgs(1);
  setMaxArgs(1);
  addContext("server");
  addContext("location");
  validate();
}

ClientBodyTempPathDirective::ClientBodyTempPathDirective(const ClientBodyTempPathDirective& other)
    : Directive(other)
    , tempPath(other.tempPath)
{
}

ClientBodyTempPathDirective& ClientBodyTempPathDirective::operator=(const ClientBodyTempPathDirective& other)
{
  if (this != &other)
  {
    Directive::operator=(other);
    tempPath = other.tempPath;
  }
  return *this;
}

bool ClientBodyTempPathDirective::validateSpecific()
{
  tempPath = getArguments()[0];
  return true;
}

void ClientBodyTempPathDirective::apply(Server& server)
{
  server.setClientBodyTempPath(tempPath);
}

void ClientBodyTempPathDirective::apply(Location& location)
{
  location.setClientBodyTempPath(tempPath);
}

void ClientBodyTempPathDirective::setTempPath(const std::string& path)
{
  this->tempPath = path;
}

const std::string& ClientBodyTempPathDirective::getTempPath() const
{
  return this->tempPath;
}
