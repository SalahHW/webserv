/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyDirective.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:51:10 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/11 06:26:15 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DenyDirective.hpp"

DenyDirective::~DenyDirective() { }

DenyDirective::DenyDirective(const std::string& currentContext, const std::string& fullDirectiveLine)
	: Directive(currentContext, fullDirectiveLine)
{
	setName("deny");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
	//TODO: Look for valid contexts for deny
	addContext("limit_except");
	validate();
}

DenyDirective::DenyDirective(const DenyDirective& other)
	: Directive(other)
{
}

DenyDirective& DenyDirective::operator=(const DenyDirective& other)
{
	if (this != &other) {
		Directive::operator=(other);
	}
	return *this;
}

bool DenyDirective::validateSpecific()
{
	//TODO: Implement validation
	return true;
}

void DenyDirective::displayInfo() const
{
	std::cout << "Name : " << this->getName() << std::endl;
}