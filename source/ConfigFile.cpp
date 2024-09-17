/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 04:06:19 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/11 04:06:19 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

using namespace std;

ConfigFile::~ConfigFile() { }

ConfigFile::ConfigFile(string const& configFilePath)
    : rootBlock("root")
{
    readConfigFile(configFilePath);

    // Debug print
    cout << endl
         << "Printing config file structure:" << endl;
    rootBlock.print();

}

const Block& ConfigFile::getRootBlock() const
{
    return (this->rootBlock);
}

void ConfigFile::readConfigFile(const string& fileName)
{
    ifstream file;
    utils::openFile(fileName, file);
    string line;

    stack<Block*> blockStack;
    blockStack.push(&rootBlock);

    while (getline(file, line)) {
        string cleanedLine = cleanLine(line);
        if (!cleanedLine.empty()) {
            handleLine(cleanedLine, file, blockStack);
        }
    }
    file.close();
}

// Function to parse the contents of a block recursively
void ConfigFile::parseBlock(ifstream& file, stack<Block*>& blockStack)
{
    string line;
    Block* currentBlock = blockStack.top();

    while (getline(file, line)) {
        string cleanedLine = cleanLine(line);
        if (cleanedLine.empty()) {
            continue;
        }

        if (cleanedLine == "}") {
            cout << "Exiting block: " << currentBlock->getName() << endl;
            blockStack.pop();
            break;
        }

        handleLine(cleanedLine, file, blockStack);
    }
}

void ConfigFile::handleLine(const string& cleanedLine, ifstream& file, stack<Block*>& blockStack)
{
    Block* currentBlock = blockStack.top();

    if (isDirective(cleanedLine)) {
        processDirective(cleanedLine, currentBlock);
    } else {
        size_t openBracePos = cleanedLine.find('{');
        string blockToParse = openBracePos == string::npos ? cleanedLine : cleanLine(cleanedLine.substr(0, openBracePos));

        if (blockToParse.empty()) {
            throw runtime_error("Unnamed block encountered.");
        }

        Block* newBlock = new Block(blockToParse, currentBlock);

        blockStack.push(newBlock);

        if (openBracePos == string::npos && !findOpeningBrace(file)) {
            throw runtime_error("Expected '{' after block name: " + blockToParse);
        } else {
            parseBlock(file, blockStack);
        }
    }
}

// Function to process a directive
void ConfigFile::processDirective(const string& cleanedLine, Block* currentBlock)
{
    if (isDirective(cleanedLine)) {
        currentBlock->addDirective(cleanedLine);
    } else {
        throw runtime_error("Unexpected content in block '" + currentBlock->getName() + "': " + cleanedLine);
    }
}

// Function to search for the opening brace '{'
bool ConfigFile::findOpeningBrace(ifstream& file)
{
    string line;
    while (getline(file, line)) {
        string cleanedLine = cleanLine(line);
        if (!cleanedLine.empty()) {
            if (cleanedLine == "{") {
                return true;
            } else {
                throw runtime_error("Expected '{' but found: " + cleanedLine);
            }
        }
    }
    throw runtime_error("End of file reached without finding '{'");
}

// Function to clean a line of comments and whitespace
string ConfigFile::cleanLine(const string& originalLine) const
{
    string cleanedLine;

    cleanedLine = utils::removeComments(originalLine);
    cleanedLine = utils::trimWhitespace(cleanedLine);
    return cleanedLine;
}

// Function to check if a line is a directive (ends with ';')
bool ConfigFile::isDirective(const string& line)
{
    return line.find(';') != string::npos;
}
