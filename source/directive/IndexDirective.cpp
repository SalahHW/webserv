/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndexDirective.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:35:32 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:37:05 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IndexDirective.hpp"

IndexDirective::~IndexDirective() { }

IndexDirective::IndexDirective(const std::string& currentContext)
	: BaseDirective(currentContext)
{
	setName("index");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
}

IndexDirective::IndexDirective(const IndexDirective& other)
	: BaseDirective(other)
	, indexPath(other.indexPath)
{
}

IndexDirective& IndexDirective::operator=(const IndexDirective& other)
{
	if (this != &other) {
		BaseDirective::operator=(other);
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

void IndexDirective::setIndexPath(const std::string& indexPath)
{
	this->indexPath = indexPath;
}

const std::string& IndexDirective::getIndexPath() const
{
	return indexPath;
}