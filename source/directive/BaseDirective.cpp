/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseDirective.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:49:56 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:03:19 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BaseDirective.hpp"

BaseDirective::~BaseDirective() { }

BaseDirective::BaseDirective(const std::string& currentContext)
    : currentContext(currentContext)
{
    tokenizeName();
}

BaseDirective::BaseDirective(const BaseDirective& other)
    : fullDirectiveLine(other.fullDirectiveLine)
    , name(other.name)
    , currentContext(other.currentContext)
    , arguments(other.arguments)
    , minArgs(other.minArgs)
    , maxArgs(other.maxArgs)
    , contexts(other.contexts)
{
}

BaseDirective& BaseDirective::operator=(const BaseDirective& other)
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

bool BaseDirective::validate()
{
    return validateContext() && validateArgsSize() && validateSpecific();
}

bool BaseDirective::validateContext() const
{
    if (std::find(contexts.begin(), contexts.end(), currentContext) == contexts.end()) {
        std::cerr << "Error: Directive '" << name << "' cannot be used in " << currentContext << std::endl;
        return false;
    }
    return true;
}

bool BaseDirective::validateArgsSize() const
{
    int argsSize = static_cast<int>(arguments.size());
    
    if (argsSize < minArgs || argsSize > maxArgs) {
        std::cerr << "Error: Directive '" << name << "' has an invalid number of arguments." << std::endl;
        return false;
    }
    return true;
}

void BaseDirective::setFullDirectiveLine(const std::string& line)
{
    this->fullDirectiveLine = line;
}

void BaseDirective::setName(const std::string& name)
{
    this->name = name;
}

void BaseDirective::setCurrentContext(const std::string& context)
{
    this->currentContext = context;
}

void BaseDirective::addArgument(const std::string& argument)
{
    this->arguments.push_back(argument);
}

void BaseDirective::addContext(const std::string& context)
{
    this->contexts.push_back(context);
}

void BaseDirective::setMinArgs(int min)
{
    this->minArgs = min;
}

void BaseDirective::setMaxArgs(int max)
{
    this->maxArgs = max;
}

std::string BaseDirective::getFullDirectiveLine() const
{
    return fullDirectiveLine;
}

std::string BaseDirective::getName() const
{
    return name;
}

std::string BaseDirective::getCurrentContext() const
{
    return currentContext;
}

std::vector<std::string> BaseDirective::getArguments() const
{
    return arguments;
}

std::vector<std::string> BaseDirective::getContexts() const
{
    return contexts;
}

int BaseDirective::getMinArgs() const
{
    return minArgs;
}

int BaseDirective::getMaxArgs() const
{
    return maxArgs;
}

void BaseDirective::tokenizeName()
{
    std::stringstream ss(fullDirectiveLine);
    std::string token;

    ss >> token;
    name = token;
    
    while (ss >> token) {
        arguments.push_back(token);
    }
}