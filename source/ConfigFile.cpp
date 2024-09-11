/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 02:14:59 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/04 02:14:59 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

ConfigFile::~ConfigFile()
{
}

ConfigFile::ConfigFile(string const& configFilePath)
{
    readConfigFile(configFilePath);
}

void ConfigFile::readConfigFile(const string& fileName)
{
    ifstream file;
    utils::openFile(fileName, file);
    string line;

    while (getline(file, line)) {
        string cleanedLine = cleanLine(line);
        if (!cleanedLine.empty())
            handleLine(cleanedLine, file, "global");
    }
    file.close();
}

// Function to parse the contents of a block recursively
void ConfigFile::parseBlock(ifstream& file, const string& blockName)
{
    string line;
    cout << "Entering block: " << blockName << endl;

    while (getline(file, line)) {
        string cleanedLine = cleanLine(line);
        if (cleanedLine.empty())
            continue; // Skip empty lines
        if (cleanedLine == "}") {
            cout << "Exiting block: " << blockName << endl;
            break;
        }
        handleLine(cleanedLine, file, blockName);
    }
}

void ConfigFile::handleLine(const string& cleanedLine, ifstream& file, const string& blockName)
{
    if (isDirective(cleanedLine)) {
        processLine(cleanedLine, blockName); // Process directive
    } else {
        // Check if block name contains '{'
        size_t openBracePos = cleanedLine.find('{');
        string blockToParse = openBracePos == string::npos ? cleanedLine : cleanLine(cleanedLine.substr(0, openBracePos));

        // If blockToParse is empty, it means there is an anonymous block, which is invalid
        if (blockToParse.empty()) {
            throw runtime_error("Unnamed block encountered.");
        }

        // If no '{', look for it
        if (openBracePos == string::npos && !findOpeningBrace(file)) {
            throw runtime_error("Expected '{' after block name: " + blockToParse);
        } else {
            parseBlock(file, blockToParse);
        }
    }
}

void ConfigFile::processLine(const string& cleanedLine, const string& blockName)
{
    if (isDirective(cleanedLine)) {
        cout << "Directive in " << blockName << ": " << cleanedLine << endl;
    } else {
        throw runtime_error("Unexpected content in block '" + blockName + "': " + cleanedLine);
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
                return true; // Found the brace
            } else {
                throw runtime_error("Expected '{' but found: " + cleanedLine);
            }
        }
    }
    throw runtime_error("End of file reached without finding '{'");
}

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