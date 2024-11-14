#pragma once

#include "Block.hpp"

class ServerBlock : public Block {
public:
	virtual ~ServerBlock();
	ServerBlock(const std::string& fullBlockLine, Block* contextBlock);
	ServerBlock(const ServerBlock& src);
	ServerBlock& operator=(const ServerBlock& src);

	void validate();
	virtual bool validateSpecific();
};
