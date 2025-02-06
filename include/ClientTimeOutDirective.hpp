#pragma once

#include "Directive.hpp"
#include "utils.hpp"

class ClientTimeOutDirective : public Directive
{
  public:
  ~ClientTimeOutDirective();
  ClientTimeOutDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ClientTimeOutDirective(const ClientTimeOutDirective& other);
  ClientTimeOutDirective& operator=(const ClientTimeOutDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);
  virtual void apply(Location& location);

  void setTimeOut(size_t time);

  size_t getTimeOut() const;

  private:
  ClientTimeOutDirective();

  int time;
};