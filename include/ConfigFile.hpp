#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "Block.hpp"
#include "BlockFactory.hpp"
#include "ConfigExtractor.hpp"
#include "ConfigFinalizer.hpp"
#include "DirectiveFactory.hpp"
#include "Port.hpp"
#include "utils.hpp"

class ConfigFile {
public:
    // Constructor and destructor
    ~ConfigFile();
    ConfigFile(const std::string& configFilePath);

    // Accessors
    bool good() const;
    const Block& getMainBlock() const;
    std::map<int, Port*> getPortMap() const;

private:
    // Disable default constructor, copy constructor, and assignment operator
    ConfigFile();
    ConfigFile(const ConfigFile& other);
    ConfigFile& operator=(const ConfigFile& other);

    // Parsing methods
    void readConfigFile(const std::string& fileName);
    void parseFile(std::ifstream& file, std::stack<Block*>& blockStack);
    bool lineEndsProperly(const std::string& line) const;
    void handleLine(const std::string& cleanedLine, std::ifstream& file, std::stack<Block*>& blockStack);
    void processBlockDeclaration(const std::string& line, std::ifstream& file, std::stack<Block*>& blockStack, Block* currentBlock);
    void parseBlock(std::ifstream& file, std::stack<Block*>& blockStack);
    void processDirective(const std::string& cleanedLine, Block* currentBlock);

    // Utility methods
    bool findOpeningBrace(std::ifstream& file, Block* currentBlock);
    std::string cleanLine(const std::string& originalLine) const;
    bool isDirective(const std::string& line) const;
    std::string extractBlockName(const std::string& line) const;
    std::string extractDirectiveName(const std::string& line) const;

    void finalizeConfig();

    // Member variables
    DirectiveFactory directiveFactory;
    BlockFactory blockFactory;
    ConfigFinalizer finalizer;
    Block* mainBlock;
    bool isValid;
    std::map<int, Port*> ports;
    std::vector<Server> servers;
};