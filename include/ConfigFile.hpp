/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 00:40:40 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/11 03:18:02 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class ConfigFile {
public:
    ~ConfigFile();
    ConfigFile(string const& configFilePath);

private:
    ConfigFile();
    ConfigFile(ConfigFile const& other);
    ConfigFile& operator=(ConfigFile const& other);

    void readConfigFile(const string& fileName);
    void parseBlock(ifstream& file, const string& blockName);
    void handleLine(const string& cleanedLine, ifstream& file, const string& blockName);
    void processLine(const string& cleanedLine, const string& blockName);
    bool findOpeningBrace(ifstream& file);
    string cleanLine(const string& originalLine) const;
    bool isDirective(const string& line);
};