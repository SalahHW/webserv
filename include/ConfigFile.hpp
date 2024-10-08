/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 00:40:40 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:00:12 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "DirectiveFactory.hpp"
#include "Block.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

class ConfigFile {
public:
    ~ConfigFile();
    ConfigFile(std::string const& configFilePath);

    const Block& getRootBlock() const;

private:
    ConfigFile();
    ConfigFile(ConfigFile const& other);
    ConfigFile& operator=(ConfigFile const& other);

    void readConfigFile(const std::string& fileName);
    void parseBlock(std::ifstream& file, std::stack<Block*>& blockStack);
    void handleLine(const std::string& cleanedLine, std::ifstream& file, std::stack<Block*>& blockStack);
    void processDirective(const std::string& cleanedLine, Block* currentBlock);
    bool findOpeningBrace(std::ifstream& file);
    std::string cleanLine(const std::string& originalLine) const;
    bool isDirective(const std::string& line);
    std::string extractDirectiveName(const std::string& line) const;

    DirectiveFactory directiveFactory;
    Block rootBlock;
};