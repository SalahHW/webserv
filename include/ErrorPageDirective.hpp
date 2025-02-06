#pragma once

#include "Directive.hpp"
#include "utils.hpp"

class ErrorPageDirective : public Directive
{
  public:
  ~ErrorPageDirective();
  ErrorPageDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ErrorPageDirective(const ErrorPageDirective& other);
  ErrorPageDirective& operator=(const ErrorPageDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);

  void setErrorPage(const std::string& errorPage);

  std::string getErrorPage() const;

  private:
  ErrorPageDirective();

  int errorCode;
  std::string errorPath;
};