/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:59:50 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/21 15:13:44 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigValidator.hpp"

using namespace std;

ConfigValidator::~ConfigValidator()
{
}

ConfigValidator::ConfigValidator(const ConfigFile& config)
    : config(config)
    , isValid(true)
{
    initValidBlocks();
    validateConfig();

    if (!isValid) {
        throw std::runtime_error("Invalid configuration");
    }
}

void ConfigValidator::initValidBlocks()
{
    initServerBlock();
    initLocationBlock();
    initLimitExceptBlock();
}

void ConfigValidator::initServerBlock()
{
    string blockName = "server";
    validBlockNames.push_back(blockName);
    validDirectivesForBlock[blockName].push_back("listen");
    validDirectivesForBlock[blockName].push_back("server_name");
    validDirectivesForBlock[blockName].push_back("client_max_body_size");
    validDirectivesForBlock[blockName].push_back("error_page");
}

void ConfigValidator::initLocationBlock()
{
    string blockName = "location";
    validBlockNames.push_back(blockName);
    validDirectivesForBlock[blockName].push_back("root");
    validDirectivesForBlock[blockName].push_back("index");
    validDirectivesForBlock[blockName].push_back("autoindex");
    validDirectivesForBlock[blockName].push_back("client_max_body_size");
    validDirectivesForBlock[blockName].push_back("return");
}

void ConfigValidator::initLimitExceptBlock()
{
    std::string blockName = "limit_except";
    validBlockNames.push_back(blockName);
    validDirectivesForBlock[blockName].push_back("deny");
}

void ConfigValidator::validateConfig()
{
    validateBlock(config.getRootBlock());
}

void ConfigValidator::validateBlock(const Block& block)
{
    std::cout << "Validating block: " << block.getName() << std::endl;
    
    if (!block.isRootBlock() && !isValidBlockName(block.getName())) {
        std::cerr << "Error: Invalid block name '" << block.getName() << "' found." << std::endl;
        isValid = false;
    }

    for (size_t i = 0; i < block.getDirectives().size(); ++i) {
        validateDirective(block.getDirectives()[i], block.getName());
    }

    for (size_t i = 0; i < block.getSubBlocks().size(); ++i) {
        validateBlock(*block.getSubBlocks()[i]);
    }
}

void ConfigValidator::validateDirective(const Directive& directive, const std::string& blockName)
{
    if (!isValidDirectiveName(directive.getName(), blockName)) {
        std::cerr << "Error: Invalid directive '" << directive.getName() << "' in block '" << blockName << "'" << std::endl;
        isValid = false;
    }
}

bool ConfigValidator::isValidBlockName(const std::string& blockName) const
{
    bool isValid = std::find(validBlockNames.begin(), validBlockNames.end(), blockName) != validBlockNames.end();

    return isValid;
}

bool ConfigValidator::isValidDirectiveName(const std::string& directiveName, const std::string& blockName) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = validDirectivesForBlock.find(blockName);
    if (it == validDirectivesForBlock.end()) {
        return false;
    }
    
    const std::vector<std::string>& validDirectives = it->second;
    return std::find(validDirectives.begin(), validDirectives.end(), directiveName) != validDirectives.end();
}