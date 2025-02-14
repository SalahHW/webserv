#pragma once

#include <set>
#include <sstream>
#include <vector>

#include "Block.hpp"
#include "Location.hpp"

class LocationBlock : public Block {
 public:
  virtual ~LocationBlock();
  LocationBlock(const std::string& fullBlockLine, Block* contextBlock);
  LocationBlock(const LocationBlock& src);
  LocationBlock& operator=(const LocationBlock& src);

  void apply(Location& location) const;

 protected:
  virtual bool validateSpecific();
};
