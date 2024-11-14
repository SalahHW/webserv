#pragma once

#include "Block.hpp"
#include "LimitExceptBlock.hpp"
#include "LocationBlock.hpp"
#include "MainBlock.hpp"
#include "ServerBlock.hpp"
#include <map>
#include <string>

class BlockFactory {
public:
    ~BlockFactory();
    BlockFactory();

    Block* create(const std::string& name, Block* contextBlock, const std::string& fullBlockLine) const;
    MainBlock* createMainBlock() const;

    template <typename T>
    void registerBlock(const std::string& name)
    {
        factoryMap[name] = &createInstance<T>;
    }

private:
    BlockFactory(const BlockFactory& other);
    BlockFactory& operator=(const BlockFactory& other);

    template <typename T>
    static Block* createInstance(Block* contextBlock, const std::string& fullBlockLine)
    {
        return new T(fullBlockLine, contextBlock);
    }

    static MainBlock* createInstance()
    {
        return new MainBlock();
    }

    std::map<std::string, Block* (*)(Block*, const std::string&)> factoryMap;
};