/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySizeDirective.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:17:39 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 17:26:18 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientMaxBodySizeDirective.hpp"

ClientMaxBodySizeDirective::~ClientMaxBodySizeDirective() { }

ClientMaxBodySizeDirective::ClientMaxBodySizeDirective(const std::string& currentContext, const std::string& fullDirectiveLine)
	: Directive(currentContext, fullDirectiveLine)
	, maxBodySize(0)
{
	setName("client_max_body_size");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("server");
	addContext("location");
	validate();

}

ClientMaxBodySizeDirective::ClientMaxBodySizeDirective(const ClientMaxBodySizeDirective& other)
	: Directive(other)
	, maxBodySize(other.maxBodySize)
{
}

ClientMaxBodySizeDirective& ClientMaxBodySizeDirective::operator=(const ClientMaxBodySizeDirective& other)
{
	if (this != &other) {
		Directive::operator=(other);
		maxBodySize = other.maxBodySize;
	}
	return *this;
}

bool ClientMaxBodySizeDirective::validateSpecific()
{
	//TODO: Implement validation
	return true;
}

void ClientMaxBodySizeDirective::displayInfo() const
{
	std::cout << "Name : " << this->getName() << std::endl << "- max body size : " << maxBodySize << std::endl;
}

void ClientMaxBodySizeDirective::apply(Server& server)
{
	server.setClientMaxBodySize(maxBodySize);
}

void ClientMaxBodySizeDirective::apply(Location& location)
{
	location.setClientMaxBodySize(maxBodySize);
}

void ClientMaxBodySizeDirective::setMaxBodySize(int maxBodySize)
{
	this->maxBodySize = maxBodySize;
}

int ClientMaxBodySizeDirective::getMaxBodySize() const
{
	return maxBodySize;
}