#pragma once

#include "Directive.hpp"
#include "utils.hpp"

class AllowMethodsDirective : public Directive
{
  public:
  ~AllowMethodsDirective();
  AllowMethodsDirective(Block* currentContext, const std::string& fullDirectiveLine);
  AllowMethodsDirective(const AllowMethodsDirective& other);
  AllowMethodsDirective& operator=(const AllowMethodsDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Location& location);

  private:
  AllowMethodsDirective();

  virtual bool isValidMethod(const std::string& method) const;
  virtual void processMethod(const std::string& method);

  bool getAllowed;
  bool postAllowed;
  bool deleteAllowed;
};