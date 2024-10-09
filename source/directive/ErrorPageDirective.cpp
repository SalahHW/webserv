/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageDirective.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:21:01 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:27:33 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPageDirective.hpp"

ErrorPageDirective::~ErrorPageDirective() { }

ErrorPageDirective::ErrorPageDirective(const std::string& currentContext)
	: BaseDirective(currentContext)
{
	setName("error_page");
	setMinArgs(2);
	//TODO: Look into the max number of arguments for error_page
	setMaxArgs(100);
	//TODO: Look for valid contexts for error_page
	addContext("server");
}

ErrorPageDirective::ErrorPageDirective(const ErrorPageDirective& other)
	: BaseDirective(other)
	, errorPages(other.errorPages)
{
}

ErrorPageDirective& ErrorPageDirective::operator=(const ErrorPageDirective& other)
{
	if (this != &other) {
		BaseDirective::operator=(other);
		errorPages = other.errorPages;
	}
	return *this;
}

bool ErrorPageDirective::validateSpecific()
{
	//TODO: Implement validation
	return true;
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