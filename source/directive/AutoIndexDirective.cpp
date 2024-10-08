/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexDirective.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:39:12 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:41:25 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndexDirective.hpp"

AutoIndexDirective::~AutoIndexDirective() { }

AutoIndexDirective::AutoIndexDirective(const std::string& currentContext)
	: BaseDirective(currentContext)
{
	setName("autoindex");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
}

AutoIndexDirective::AutoIndexDirective(const AutoIndexDirective& other)
	: BaseDirective(other)
	, autoIndex(other.autoIndex)
{
}

AutoIndexDirective& AutoIndexDirective::operator=(const AutoIndexDirective& other)
{
	if (this != &other) {
		BaseDirective::operator=(other);
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

void AutoIndexDirective::setAutoIndex(bool autoIndex)
{
	this->autoIndex = autoIndex;
}

bool AutoIndexDirective::getAutoIndex() const
{
	return autoIndex;
}

