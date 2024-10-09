/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyDirective.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:51:10 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/09 12:01:48 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DenyDirective.hpp"

DenyDirective::~DenyDirective() { }

DenyDirective::DenyDirective(const std::string& currentContext)
	: Directive(currentContext)
{
	setName("deny");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
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