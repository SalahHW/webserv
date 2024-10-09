/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:49:56 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/09 12:01:18 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"

Directive::~Directive() { }

Directive::Directive(const std::string& currentContext)
    : currentContext(currentContext)
{
    tokenizeName();
}

Directive::Directive(const Directive& other)
    : fullDirectiveLine(other.fullDirectiveLine)
    , name(other.name)
    , currentContext(other.currentContext)
    , arguments(other.arguments)
    , minArgs(other.minArgs)
    , maxArgs(other.maxArgs)
    , contexts(other.contexts)
{
}

Directive& Directive::operator=(const Directive& other)
{
    if (this != &other) {
        fullDirectiveLine = other.fullDirectiveLine;
        name = other.name;
        currentContext = other.currentContext;
        arguments = other.arguments;
        minArgs = other.minArgs;
        maxArgs = other.maxArgs;
        contexts = other.contexts;
    }
    return *this;
}

bool Directive::validate()
{
    return validateContext() && validateArgsSize() && validateSpecific();
}

bool Directive::validateContext() const
{
    if (std::find(contexts.begin(), contexts.end(), currentContext) == contexts.end()) {
        std::cerr << "Error: Directive '" << name << "' cannot be used in " << currentContext << std::endl;
        return false;
    }
    return true;
}

bool Directive::validateArgsSize() const
{
    int argsSize = static_cast<int>(arguments.size());
    
    if (argsSize < minArgs || argsSize > maxArgs) {
        std::cerr << "Error: Directive '" << name << "' has an invalid number of arguments." << std::endl;
        return false;
    }
    return true;
}

void Directive::setFullDirectiveLine(const std::string& line)
{
    this->fullDirectiveLine = line;
}

void Directive::setName(const std::string& name)
{
    this->name = name;
}

void Directive::setCurrentContext(const std::string& context)
{
    this->currentContext = context;
}

void Directive::addArgument(const std::string& argument)
{
    this->arguments.push_back(argument);
}

void Directive::addContext(const std::string& context)
{
    this->contexts.push_back(context);
}

void Directive::setMinArgs(int min)
{
    this->minArgs = min;
}

void Directive::setMaxArgs(int max)
{
    this->maxArgs = max;
}

std::string Directive::getFullDirectiveLine() const
{
    return fullDirectiveLine;
}

std::string Directive::getName() const
{
    return name;
}

std::string Directive::getCurrentContext() const
{
    return currentContext;
}

std::vector<std::string> Directive::getArguments() const
{
    return arguments;
}

std::vector<std::string> Directive::getContexts() const
{
    return contexts;
}

int Directive::getMinArgs() const
{
    return minArgs;
}

int Directive::getMaxArgs() const
{
    return maxArgs;
}

void Directive::tokenizeName()
{
    std::stringstream ss(fullDirectiveLine);
    std::string token;

    ss >> token;
    name = token;
    
    while (ss >> token) {
        arguments.push_back(token);
    }
}