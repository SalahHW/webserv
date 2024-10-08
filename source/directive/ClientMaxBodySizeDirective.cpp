/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySizeDirective.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:17:39 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:19:13 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientMaxBodySizeDirective.hpp"

ClientMaxBodySizeDirective::~ClientMaxBodySizeDirective() { }

ClientMaxBodySizeDirective::ClientMaxBodySizeDirective(const std::string& currentContext)
	: BaseDirective(currentContext)
	, maxBodySize(0)
{
	setName("client_max_body_size");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("server");
	addContext("location");
}

ClientMaxBodySizeDirective::ClientMaxBodySizeDirective(const ClientMaxBodySizeDirective& other)
	: BaseDirective(other)
	, maxBodySize(other.maxBodySize)
{
}

ClientMaxBodySizeDirective& ClientMaxBodySizeDirective::operator=(const ClientMaxBodySizeDirective& other)
{
	if (this != &other) {
		BaseDirective::operator=(other);
		maxBodySize = other.maxBodySize;
	}
	return *this;
}

bool ClientMaxBodySizeDirective::validateSpecific()
{
	//TODO: Implement validation
	return true;
}

void ClientMaxBodySizeDirective::setMaxBodySize(int maxBodySize)
{
	this->maxBodySize = maxBodySize;
}

int ClientMaxBodySizeDirective::getMaxBodySize() const
{
	return maxBodySize;
}