/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnDirective.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:55:01 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:58:39 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReturnDirective.hpp"

ReturnDirective::~ReturnDirective() { }

ReturnDirective::ReturnDirective(const std::string& currentContext)
	: BaseDirective(currentContext)
{
	setName("return");
	//TODO: Check min and max number of arguments for return directive
	setMinArgs(2);
	setMaxArgs(2);
	addContext("location");
}

ReturnDirective::ReturnDirective(const ReturnDirective& other)
	: BaseDirective(other)
	, returnCode(other.returnCode)
	, returnPath(other.returnPath)
{
}

ReturnDirective& ReturnDirective::operator=(const ReturnDirective& other)
{
	if (this != &other) {
		BaseDirective::operator=(other);
		returnCode = other.returnCode;
		returnPath = other.returnPath;
	}
	return *this;
}

bool ReturnDirective::validateSpecific()
{
	//TODO: Implement validation for return directive
	return true;
}

void ReturnDirective::setReturnCode(int returnCode)
{
	this->returnCode = returnCode;
}

void ReturnDirective::setReturnPath(const std::string& path)
{
	this->returnPath = path;
}

int ReturnDirective::getReturnCode() const
{
	return returnCode;
}

const std::string &ReturnDirective::getReturnPath() const
{
	return returnPath;
}