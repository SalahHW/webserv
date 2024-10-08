/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseDirective.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:49:56 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 11:23:13 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BaseDirective.hpp"

BaseDirective::~BaseDirective() { }

BaseDirective::BaseDirective(const std::string& currentContext)
    : currentContext(currentContext)
{
}

BaseDirective::BaseDirective(const BaseDirective& other)
    : name(other.name)
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
    if (arguments.size() < minArgs || arguments.size() > maxArgs) {
        std::cerr << "Error: Directive '" << name << "' has an invalid number of arguments." << std::endl;
        return false;
    }
    return true;
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