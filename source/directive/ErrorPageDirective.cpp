/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageDirective.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:21:01 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/14 15:02:55 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageDirective.hpp"

ErrorPageDirective::~ErrorPageDirective() { }

ErrorPageDirective::ErrorPageDirective(Block* currentContext, const std::string& fullDirectiveLine)
	: Directive(currentContext, fullDirectiveLine)
{
	setName("error_page");
	setMinArgs(2);
	//TODO: Look into the max number of arguments for error_page
	setMaxArgs(100);
	//TODO: Look for valid contexts for error_page
	addContext("server");
	validate();
}

ErrorPageDirective::ErrorPageDirective(const ErrorPageDirective& other)
	: Directive(other)
	, errorPages(other.errorPages)
{
}

ErrorPageDirective& ErrorPageDirective::operator=(const ErrorPageDirective& other)
{
	if (this != &other) {
		Directive::operator=(other);
		errorPages = other.errorPages;
	}
	return *this;
}

bool ErrorPageDirective::validateSpecific()
{
	//TODO: Implement validation
	return true;
}

void ErrorPageDirective::displayInfo() const
{
	std::cout << "Name : " << this->getName() << std::endl;
}

void ErrorPageDirective::apply(Server& server)
{
	server.setErrorPages(errorPages);
}

void ErrorPageDirective::setErrorPage(const std::string& errorPage)
{
	//TODO: Implement setErrorPage
	(void)errorPage;
}

std::string ErrorPageDirective::getErrorPage() const
{
	//TODO: Implement getErrorPage
	return "";
}