/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootDirective.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:28:45 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 17:27:07 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RootDirective.hpp"

RootDirective::~RootDirective() { }

RootDirective::RootDirective(const std::string& currentContext, const std::string& fullDirectiveLine)
	: Directive(currentContext, fullDirectiveLine)
{
	setName("root");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
	validate();
}

RootDirective::RootDirective(const RootDirective& other)
	: Directive(other)
	, rootPath(other.rootPath)
{
}

RootDirective& RootDirective::operator=(const RootDirective& other)
{
	if (this != &other) {
		Directive::operator=(other);
		rootPath = other.rootPath;
	}
	return *this;
}

bool RootDirective::validateSpecific()
{
	rootPath = getArguments()[0];
	//TODO: Implement validation
	return true;
}

void RootDirective::displayInfo() const
{
	std::cout << "Name : " << this->getName() << std::endl << "- root path : " << rootPath << std::endl;
}

void RootDirective::apply(Location& location)
{
	location.setRootDirectory(rootPath);
}

void RootDirective::setRootPath(const std::string& rootPath)
{
	this->rootPath = rootPath;
}

const std::string& RootDirective::getRootPath() const
{
	return rootPath;
}