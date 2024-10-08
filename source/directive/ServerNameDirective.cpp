/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNameDirective.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:08:07 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:11:55 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerNameDirective.hpp"

ServerNameDirective::~ServerNameDirective() { }

ServerNameDirective::ServerNameDirective(const std::string& currentContext)
    : BaseDirective(currentContext)
    , serverName("default")
{
    setName("server_name");
    setMinArgs(1);
    setMaxArgs(1);
    addContext("server");
}

ServerNameDirective::ServerNameDirective(const ServerNameDirective& other)
    : BaseDirective(other)
    , serverName(other.serverName)
{
}

ServerNameDirective& ServerNameDirective::operator=(const ServerNameDirective& other)
{
    if (this != &other) {
        BaseDirective::operator=(other);
        serverName = other.serverName;
    }
    return *this;
}

bool ServerNameDirective::validateSpecific()
{
    serverName = getArguments()[0];
	if (serverName.empty()) {
		std::cerr << "Error: Directive 'server_name' in " << getCurrentContext() << " has an empty server name." << std::endl;
		return false;
	}
	return true;
}

void ServerNameDirective::setServerName(const std::string& serverName)
{
	this->serverName = serverName;
}

const std::string& ServerNameDirective::getServername() const
{
	return serverName;
}