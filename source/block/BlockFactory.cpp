#include "BlockFactory.hpp"

BlockFactory::~BlockFactory() { }

BlockFactory::BlockFactory()
{
  registerBlock<ServerBlock>("server");
  registerBlock<LocationBlock>("location");
}

Block* BlockFactory::create(const std::string& name, Block* contextBlock, const std::string& fullBlockLine) const
{
  std::map<std::string,
      Block* (*)(Block*, const std::string&)>::const_iterator it
      = factoryMap.find(name);
  if (it != factoryMap.end())
  {
    return it->second(contextBlock, fullBlockLine);
  }
  return NULL;
}

MainBlock* BlockFactory::createMainBlock() const
{
  return new MainBlock();
}
