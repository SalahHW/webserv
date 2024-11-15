#pragma once

#include "Block.hpp"
#include "Location.hpp"
#include <set>
#include <sstream>
#include <vector>

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
