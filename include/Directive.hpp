/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 07:58:54 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/17 13:18:30 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Directive {
public:
    ~Directive();
    Directive(const std::string& fullDirectiveLine);
    Directive(Directive const& other);
    Directive& operator=(Directive const& other);

    void addArgument(const std::string& argument);

    std::string const& getName() const;
    std::string const& getFullDirectiveLine() const;
    std::vector<std::string> const& getArguments() const;

private:
    Directive();

    void tokenizeName();

    std::string fullDirectiveLine;
    std::string name;
    std::vector<std::string> arguments;
};