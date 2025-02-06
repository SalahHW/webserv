#pragma once

#include "Directive.hpp"

class DefaultServerDirective : public Directive {
 public:
  ~DefaultServerDirective();
  DefaultServerDirective(Block* currentContext,
                         const std::string& fullDirectiveLine);
  DefaultServerDirective(const DefaultServerDirective& other);
  DefaultServerDirective& operator=(const DefaultServerDirective& other);

  virtual bool validateSpecific();
  virtual void displayInfo() const;
  virtual void apply(Server& server);

 private:
  DefaultServerDirective();
};