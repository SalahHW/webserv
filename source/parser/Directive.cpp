/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 05:40:12 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/30 11:18:08 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"

using namespace std;

Directive::~Directive()
{
}

Directive::Directive(const string& fullDirectiveLine)
    : fullDirectiveLine(fullDirectiveLine)
{
    tokenizeName();
}

Directive::Directive(Directive const& other)
{
    fullDirectiveLine = other.fullDirectiveLine;
    name = other.name;
    arguments = other.arguments;
}

Directive& Directive::operator=(Directive const& other)
{
    if (this != &other) {
        fullDirectiveLine = other.fullDirectiveLine;
        name = other.name;
        arguments = other.arguments;
    }
    return (*this);
}

void Directive::addArgument(const string& argument)
{
    arguments.push_back(argument);
}

string const& Directive::getName() const
{
    return (name);
}

string const& Directive::getFullDirectiveLine() const
{
    return (fullDirectiveLine);
}

vector<string> const& Directive::getArguments() const
{
    return (arguments);
}

void Directive::tokenizeName()
{
    stringstream ss(fullDirectiveLine);
    string token;

    ss >> token;
    name = token;
    cout << "Name: " << name << endl;
    
    while (ss >> token) {
        if (!token.empty() && token[token.size() - 1] == ';') {
            token = token.substr(0, token.size() - 1);
        }
        arguments.push_back(token);
        cout << "Argument: " << token << endl;
    }
}