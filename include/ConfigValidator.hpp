/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:56:08 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/02 10:41:14 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ConfigFile.hpp"
#include "DirectiveValidator.hpp"
#include <algorithm>
#include <map>
#include <string>
#include <vector>

class ConfigValidator {
public:
    ~ConfigValidator();
    ConfigValidator(const ConfigFile& config);

private:
    ConfigValidator();
    ConfigValidator(const ConfigValidator& other);
    ConfigValidator& operator=(const ConfigValidator& other);

    void initValidBlocks();
    void initServerBlock();
    void initLocationBlock();
    void initLimitExceptBlock();
    void validateConfig();
    void validateBlock(const Block& block);
    void validateDirective(const Directive& directive, const std::string& blockName);
    bool isValidBlockName(const std::string& blockName) const;
    bool isValidDirectiveName(const std::string& directiveName, const std::string& blockName) const;

    const ConfigFile& config;
    bool isValid;
    std::map<std::string, std::vector<std::string> > validDirectivesForBlock;
    std::vector<std::string> validBlockNames;
    DirectiveValidator directiveValidator;
};