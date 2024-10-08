/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootDirective.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:28:45 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:32:42 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RootDirective.hpp"

RootDirective::~RootDirective() { }

RootDirective::RootDirective(const std::string& currentContext)
	: BaseDirective(currentContext)
{
	setName("root");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
}

RootDirective::RootDirective(const RootDirective& other)
	: BaseDirective(other)
	, rootPath(other.rootPath)
{
}

RootDirective& RootDirective::operator=(const RootDirective& other)
{
	if (this != &other) {
		BaseDirective::operator=(other);
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

void RootDirective::setRootPath(const std::string& rootPath)
{
	this->rootPath = rootPath;
}

const std::string& RootDirective::getRootPath() const
{
	return rootPath;
}