/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigValidator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:56:08 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/12 14:33:51 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ConfigFile.hpp"
#include <algorithm>
#include <string>
#include <vector>

class ConfigValidator {
public:
    ~ConfigValidator();
    ConfigValidator(const ConfigFile& config);

    void validateConfig();

private:
    ConfigValidator(const ConfigValidator& other);
    ConfigValidator& operator=(const ConfigValidator& other);

    void validateBlock(const Block& block);
    bool isValidBlockName(const std::string& blockName) const;

    const ConfigFile& config;
    bool isValid;
};