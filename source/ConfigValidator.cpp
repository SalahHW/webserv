/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:59:50 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/16 07:38:52 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigValidator.hpp"

ConfigValidator::~ConfigValidator()
{
}

ConfigValidator::ConfigValidator(const ConfigFile& config)
    : config(config), isValid(true)
{
}

void ConfigValidator::validateConfig()
{
    validateBlock(config.getRootBlock());
}

void ConfigValidator::validateBlock(const Block& block)
{
    if (!isValidBlockName(block.getName())) {
        std::cout << "Invalid block name: " << block.getName() << std::endl;
		isValid = false;
    }
}

bool ConfigValidator::isValidBlockName(const std::string& blockName) const
{
    std::vector<std::string> validBlockNames;
	validBlockNames.push_back("server");
	validBlockNames.push_back("location");
	validBlockNames.push_back("root");
    validBlockNames.push_back("limit_except GET POST");
    return (std::find(validBlockNames.begin(), validBlockNames.end(), blockName) != validBlockNames.end());
}