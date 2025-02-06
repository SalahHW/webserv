#pragma once

#include "Directive.hpp"
#include "utils.hpp"

class ListenDirective : public Directive {
 public:
  ~ListenDirective();
  ListenDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ListenDirective(const ListenDirective& other);
  ListenDirective& operator=(const ListenDirective& other);

  virtual bool validateSpecific();
  virtual void displayInfo() const;
  virtual void apply(Server& server);

  void setPort(int port);

  int getPort() const;

 private:
  ListenDirective();

  int port;
};