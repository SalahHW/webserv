/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexDirective.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:39:12 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 11:11:20 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndexDirective.hpp"

AutoIndexDirective::~AutoIndexDirective() { }

AutoIndexDirective::AutoIndexDirective(const std::string& currentContext)
	: Directive(currentContext)
{
	setName("autoindex");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
}

AutoIndexDirective::AutoIndexDirective(const AutoIndexDirective& other)
	: Directive(other)
	, autoIndex(other.autoIndex)
{
}

AutoIndexDirective& AutoIndexDirective::operator=(const AutoIndexDirective& other)
{
	if (this != &other) {
		Directive::operator=(other);
		autoIndex = other.autoIndex;
	}
	return *this;
}

bool AutoIndexDirective::validateSpecific()
{
	if (getArguments()[0] == "on") {
		autoIndex = true;
	} else if (getArguments()[0] == "off") {
		autoIndex = false;
	} else {
		std::cerr << "Error: Directive 'autoindex' in " << getCurrentContext() << " has an invalid argument." << std::endl;
		return false;
	}
	return true;
}

void AutoIndexDirective::displayInfo() const
{
	std::cout << "Name : " << this->getName() << " autoindex : " << autoIndex << std::endl;
}

void AutoIndexDirective::apply(Location& location)
{
	location.setAutoIndex(autoIndex);
}

void AutoIndexDirective::setAutoIndex(bool autoIndex)
{
	this->autoIndex = autoIndex;
}

bool AutoIndexDirective::getAutoIndex() const
{
	return autoIndex;
}

