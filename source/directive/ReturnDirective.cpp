/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnDirective.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:55:01 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/14 16:13:01 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReturnDirective.hpp"

ReturnDirective::~ReturnDirective() { }

ReturnDirective::ReturnDirective(Block* currentContext, const std::string& fullDirectiveLine)
	: Directive(currentContext, fullDirectiveLine)
{
	setName("return");
	//TODO: Check min and max number of arguments for return directive
	setMinArgs(2);
	setMaxArgs(2);
	addContext("location");
	validate();
}

ReturnDirective::ReturnDirective(const ReturnDirective& other)
	: Directive(other)
	, returnCode(other.returnCode)
	, returnPath(other.returnPath)
{
}

ReturnDirective& ReturnDirective::operator=(const ReturnDirective& other)
{
	if (this != &other) {
		Directive::operator=(other);
		returnCode = other.returnCode;
		returnPath = other.returnPath;
	}
	return *this;
}

bool ReturnDirective::validateSpecific()
{
	if (!utils::convertToInt(arguments[0].c_str(), returnCode)) {
		std::cerr << "Error: Directive \"" << getName() << "\" has an invalid argument." << std::endl;
		return false;
	}
	returnPath = getArguments()[1];
	return true;
}

void ReturnDirective::displayInfo() const
{
	std::cout << "Name : " << this->getName() << std::endl << "- return code : " << returnCode << std::endl << "- return path : " << returnPath << std::endl;
}

void ReturnDirective::apply(Location& location)
{
	location.setReturnCode(returnCode);
	location.setRedirectionPath(returnPath);
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
