/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveValidator.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:47:50 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/03 10:23:42 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <map>
#include <cstdlib>

class DirectiveValidator {
public:
    ~DirectiveValidator();
    DirectiveValidator();

	bool validate(const Directive& directive);
private:
    typedef bool (DirectiveValidator::*ValidationFunction)(const Directive&);

    std::map<std::string, ValidationFunction> validationFunctions;

    // Directive validation functions
    bool validateListen(const Directive& directive);
    bool validateServerName(const Directive& directive);
	bool validateClientMaxBodySize(const Directive& directive);
    bool validateErrorPage(const Directive& directive);
    bool validateRoot(const Directive& directive);
    bool validateIndex(const Directive& directive);
    bool validateAutoIndex(const Directive& directive);

    // bool isNumber(const std::string& str) const;
    // bool isBoolean(const std::string& str) const;
    // bool isValidPath(const std::string& str) const;
    bool isValidSize(const std::string& str) const;
};