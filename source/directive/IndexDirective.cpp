/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndexDirective.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:35:32 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 17:26:36 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IndexDirective.hpp"

IndexDirective::~IndexDirective() { }

IndexDirective::IndexDirective(const std::string& currentContext, const std::string& fullDirectiveLine)
	: Directive(currentContext, fullDirectiveLine)
{
	setName("index");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
	validate();
}

IndexDirective::IndexDirective(const IndexDirective& other)
	: Directive(other)
	, indexPath(other.indexPath)
{
}

IndexDirective& IndexDirective::operator=(const IndexDirective& other)
{
	if (this != &other) {
		Directive::operator=(other);
		indexPath = other.indexPath;
	}
	return *this;
}

bool IndexDirective::validateSpecific()
{
	indexPath = getArguments()[0];
	//TODO: Implement validation
	return true;
}

void IndexDirective::displayInfo() const
{
	std::cout << "Name : " << this->getName() << std::endl << "- index path : " << indexPath << std::endl;
}

void IndexDirective::apply(Location& location)
{
	location.setIndexFile(indexPath);
}

void IndexDirective::setIndexPath(const std::string& indexPath)
{
	this->indexPath = indexPath;
}

const std::string& IndexDirective::getIndexPath() const
{
	return indexPath;
}