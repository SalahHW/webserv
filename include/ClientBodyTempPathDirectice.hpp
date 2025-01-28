#pragma once

#include "Directive.hpp"

class ClientBodyTempPathDirective : public Directive
{
  public:
  ~ClientBodyTempPathDirective();
  ClientBodyTempPathDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ClientBodyTempPathDirective(const ClientBodyTempPathDirective& other);
  ClientBodyTempPathDirective& operator=(const ClientBodyTempPathDirective& other);

  virtual bool validateSpecific();
  virtual void displayInfo() const;
  virtual void apply(Server& server);
  virtual void apply(Location& location);

  void setTempPath(const std::string& path);

  const std::string& getTempPath() const;

  private:
  ClientBodyTempPathDirective();

  std::string tempPath;
};