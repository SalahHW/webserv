#pragma once

#include "Directive.hpp"

class AutoIndexDirective : public Directive {
 public:
  ~AutoIndexDirective();
  AutoIndexDirective(Block* currentContext,
                     const std::string& fullDirectiveLine);
  AutoIndexDirective(const AutoIndexDirective& other);
  AutoIndexDirective& operator=(const AutoIndexDirective& other);

  virtual bool validateSpecific();
  virtual void displayInfo() const;
  virtual void apply(Location& location);

  void setAutoIndex(bool autoIndex);

  bool getAutoIndex() const;

 private:
  AutoIndexDirective();

  bool autoIndex;
};