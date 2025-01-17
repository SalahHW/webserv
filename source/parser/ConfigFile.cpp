#include "ConfigFile.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>

ConfigFile::ConfigFile(const std::string& configFilePath)
    : mainBlock(NULL)
    , isValid(true)
{
    readConfigFile(configFilePath);
    servers = ConfigExtractor::extractServers(*this);
    finalizeConfig();
}

ConfigFile::~ConfigFile()
{
    delete mainBlock;
    std::map<int, Port*>::iterator itPort;

    for (itPort = ports.begin(); itPort != ports.end(); ++itPort) {
        delete itPort->second;
    }
}

std::map<int, Port*> ConfigFile::getPortMap() const
{
    return this->ports;
}

const Block& ConfigFile::getMainBlock() const { return *mainBlock; }

bool ConfigFile::good() const { return isValid; }

void ConfigFile::readConfigFile(const std::string& fileName)
{
    std::ifstream file;
    utils::openFile(fileName, file);

    mainBlock = BlockFactory().createMainBlock();
    std::stack<Block*> blockStack;
    blockStack.push(mainBlock);

    parseFile(file, blockStack);

    file.close();

    if (isValid && blockStack.size() > 1) {
        std::cerr << "Error: Some blocks were not closed properly." << std::endl;
        while (blockStack.size() > 1) {
            Block* unclosedBlock = blockStack.top();
            std::cerr << "Unclosed block: \"" << unclosedBlock->getName() << "\"" << std::endl;
            blockStack.pop();
        }
        isValid = false;
    }
}

void ConfigFile::parseFile(std::ifstream& file,
    std::stack<Block*>& blockStack)
{
    std::string line;
    std::string accumulatedLine;

    while (std::getline(file, line)) {
        std::string cleanedLine = cleanLine(line);

        if (!cleanedLine.empty()) {
            accumulatedLine += cleanedLine + " ";
            if (lineEndsProperly(accumulatedLine)) {
                handleLine(accumulatedLine, file, blockStack);
                accumulatedLine.clear();
                if (!isValid) {
                    break;
                }
            }
        }
    }
    if (!accumulatedLine.empty()) {
        handleLine(accumulatedLine, file, blockStack);
    }
}

bool ConfigFile::lineEndsProperly(const std::string& line) const
{
    std::string trimmedLine = utils::trimWhitespace(line);
    if (trimmedLine.empty()) {
        return false;
    }
    char lastChar = trimmedLine[trimmedLine.size() - 1];
    return (lastChar == ';' || lastChar == '{' || lastChar == '}');
}

void ConfigFile::handleLine(const std::string& accumulatedLine,
    std::ifstream& file,
    std::stack<Block*>& blockStack)
{
    std::string cleanedLine = utils::trimWhitespace(accumulatedLine);
    Block* currentBlock = blockStack.top();

    if (cleanedLine.empty()) {
        return;
    }
    if (cleanedLine == "{") {
    } else if (cleanedLine == "}") {
        if (blockStack.size() > 1) {
            blockStack.pop();
        } else {
            std::cerr << "Error: Unmatched closing brace '}' in block \""
                      << currentBlock->getName() << "\"." << std::endl;
            isValid = false;
        }
    } else if (isDirective(cleanedLine)) {
        processDirective(cleanedLine, currentBlock);
    } else {
        bool onlyClosingBraces = true;
        for (size_t i = 0; i < cleanedLine.size(); ++i) {
            if (cleanedLine[i] != '}') {
                onlyClosingBraces = false;
                break;
            }
        }

        if (onlyClosingBraces) {
            for (size_t i = 0; i < cleanedLine.size(); ++i) {
                if (blockStack.size() > 1) {
                    blockStack.pop();
                } else {
                    std::cerr << "Error: Unmatched closing brace '}' in block \""
                              << currentBlock->getName() << "\"." << std::endl;
                    isValid = false;
                    return;
                }
            }
        } else {
            processBlockDeclaration(cleanedLine, file, blockStack, currentBlock);
        }
    }
}

void ConfigFile::processBlockDeclaration(const std::string& line,
    std::ifstream& file,
    std::stack<Block*>& blockStack,
    Block* currentBlock)
{
    std::string blockLine = line;
    std::string afterBlockName;
    bool braceFoundInLine = false;
    std::size_t openBracePos = line.find('{');
    if (openBracePos != std::string::npos) {
        braceFoundInLine = true;
        blockLine = line.substr(0, openBracePos);
        afterBlockName = line.substr(openBracePos + 1);
    }
    std::string blockName = extractBlockName(blockLine);

    if (blockName.empty()) {
        std::cerr << "Error: Unnamed block encountered inside block \""
                  << currentBlock->getName() << "\"." << std::endl;
        isValid = false;
        return;
    }

    if (!braceFoundInLine) {
        if (!findOpeningBrace(file, currentBlock)) {
            std::cerr << "Error: Expected '{' after block name \"" << blockName
                      << "\" inside block \"" << currentBlock->getName() << "\"."
                      << std::endl;
            isValid = false;
            return;
        }
    } else {
        std::string restLine = cleanLine(afterBlockName);
        if (!restLine.empty()) {
            std::cerr << "Error: Unexpected content after '{' in block \""
                      << blockName << "\" inside block \"" << currentBlock->getName()
                      << "\"." << std::endl;
            isValid = false;
            return;
        }
    }

    std::string cleanedLine = utils::removeBraces(line);
    Block* newBlock = BlockFactory().create(blockName, currentBlock, cleanedLine);

    if (!newBlock) {
        std::cerr << "Error: Block \"" << blockName << "\" in block \""
                  << currentBlock->getName() << "\" is not supported." << std::endl;
        isValid = false;
        return;
    }

    if (!newBlock->good()) {
        delete newBlock;
        isValid = false;
        return;
    }
    currentBlock->addSubBlock(newBlock);
    blockStack.push(newBlock);
    parseBlock(file, blockStack);
}

void ConfigFile::parseBlock(std::ifstream& file, std::stack<Block*>& blockStack)
{
    std::string line;
    std::string accumulatedLine;

    while (std::getline(file, line)) {
        std::string cleanedLine = cleanLine(line);

        if (!cleanedLine.empty()) {
            accumulatedLine += cleanedLine + " ";
            if (lineEndsProperly(accumulatedLine)) {
                handleLine(accumulatedLine, file, blockStack);
                accumulatedLine.clear();
                if (!isValid) {
                    return;
                }
            }
        }
    }
    if (!accumulatedLine.empty()) {
        handleLine(accumulatedLine, file, blockStack);
    }
    if (blockStack.size() > 1) {
        std::cerr << "Error: Block \"" << blockStack.top()->getName()
                  << "\" not closed properly (missing '}' before EOF)."
                  << std::endl;
        isValid = false;
    }
}

void ConfigFile::processDirective(const std::string& cleanedLine,
    Block* currentBlock)
{
    if (cleanedLine.empty()) {
        std::cerr << "Unexpected empty directive in block \""
                  << currentBlock->getName() << "\"." << std::endl;
        isValid = false;
        return;
    }

    if (cleanedLine[cleanedLine.size() - 1] != ';') {
        std::cerr << "Error: Missing ';' at the end of directive \"" << cleanedLine
                  << "\" in block \"" << currentBlock->getName() << "\"."
                  << std::endl;
        isValid = false;
        return;
    }

    std::string directiveLine = cleanedLine.substr(0, cleanedLine.size() - 1);
    std::string directiveName = extractDirectiveName(directiveLine);

    Directive* directive = directiveFactory.create(directiveName, currentBlock, directiveLine);
    if (!directive) {
        std::cerr << "Error: Directive \"" << directiveName << "\" in block \""
                  << currentBlock->getName() << "\" is not supported." << std::endl;
        isValid = false;
        return;
    }
    if (!directive->good()) {
        delete directive;
        isValid = false;
        return;
    }
    currentBlock->addDirective(directive);
}

bool ConfigFile::isDirective(const std::string& line) const
{
    std::string trimmedLine = utils::trimWhitespace(line);
    if (trimmedLine.empty()) {
        return false;
    }

    if (trimmedLine[trimmedLine.size() - 1] != ';') {
        return false;
    }

    std::size_t semiColonPos = trimmedLine.find_last_of(';');
    std::size_t bracePos = trimmedLine.find('{');

    if (bracePos != std::string::npos && bracePos < semiColonPos) {
        return false;
    }

    return true;
}

bool ConfigFile::findOpeningBrace(std::ifstream& file, Block* currentBlock)
{
    std::string line;
    while (std::getline(file, line)) {
        std::string cleanedLine = cleanLine(line);
        if (!cleanedLine.empty()) {
            if (cleanedLine == "{") {
                return true;
            } else {
                std::cerr << "Expected '{' but found: \"" << cleanedLine
                          << "\" inside block \"" << currentBlock->getName() << "\"."
                          << std::endl;
                isValid = false;
                return false;
            }
        }
    }
    std::cerr << "End of file reached without finding '{' inside block \""
              << currentBlock->getName() << "\"." << std::endl;
    isValid = false;
    return false;
}

std::string ConfigFile::cleanLine(const std::string& originalLine) const
{
    std::string cleanedLine = utils::removeComments(originalLine);
    cleanedLine = utils::trimWhitespace(cleanedLine);
    return cleanedLine;
}

std::string ConfigFile::extractBlockName(const std::string& line) const
{
    std::stringstream ss(line);
    std::string blockName;
    ss >> blockName;
    return blockName;
}

std::string ConfigFile::extractDirectiveName(const std::string& line) const
{
    std::stringstream ss(line);
    std::string directiveName;
    ss >> directiveName;
    return directiveName;
}

void ConfigFile::finalizeConfig()
{
    finalizer.finalizeConfig(servers);
    if (!finalizer.good()) {
        std::cerr << "Error: Invalid port configuration" << std::endl;
        isValid = false;
        return;
    }
    ports = finalizer.getPorts();
}