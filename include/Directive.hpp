/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 07:58:54 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/17 06:12:59 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <iostream>

class Directive {
public:
    ~Directive();
    Directive(const std::string& fullDirectiveLine);
    Directive(Directive const& other);
    Directive& operator=(Directive const& other);

	void addArgument(const std::string& argument);
	
	std::string const& getName() const;
	std::vector<std::string> const& getArguments() const;

private:
    Directive();

	void tokenizeName();

	std::string fullDirectiveLine;
    std::string name;
    std::vector<std::string> arguments;
};