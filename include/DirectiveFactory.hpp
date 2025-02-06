#pragma once

#include "Directive.hpp"
#include <map>
#include <string>

class DirectiveFactory
{
  public:
  ~DirectiveFactory();
  DirectiveFactory();

  Directive* create(const std::string& name, Block* context, const std::string& fullDirectiveline) const;

  template <typename T>
  void registerDirective(const std::string& name)
  {
    factoryMap[name] = &createInstance<T>;
  }

  private:
  DirectiveFactory(const DirectiveFactory& other);
  DirectiveFactory& operator=(const DirectiveFactory& other);

  template <typename T>
  static Directive* createInstance(Block* context, const std::string& fullDirectiveLine)
  {
    return new T(context, fullDirectiveLine);
  }

  std::map<std::string, Directive* (*)(Block*, const std::string&)> factoryMap;
};