/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenDirective.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:16:08 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 11:23:36 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListenDirective.hpp"

ListenDirective::~ListenDirective() { }

ListenDirective::ListenDirective(const std::string& currentContext)
    : BaseDirective(currentContext)
    , port(0)
{
    setName("listen");
    setMinArgs(1);
    setMaxArgs(1);
    addContext("server");
}

ListenDirective::ListenDirective(const ListenDirective& other)
    : BaseDirective(other)
    , port(other.port)
{
}

ListenDirective& ListenDirective::operator=(const ListenDirective& other)
{
    if (this != &other) {
        BaseDirective::operator=(other);
        port = other.port;
    }
    return *this;
}

bool ListenDirective::validateSpecific()
{
    if (!utils::isIntCompatible(getArguments()[0])) {
        std::cerr << "Error: Directive 'listen' in " << getCurrentContext() << " has an invalid port number." << std::endl;
        return false;
    }
    this->port = atoi(getArguments()[0].c_str());
    if (port < 0 || port > 65535) {
        std::cerr << "Error: Directive 'listen' in " << getCurrentContext() << " has a port number out of range. (0 - 65535)" << std::endl;
        return false;
    }
    return true;
}

void ListenDirective::setPort(int port)
{
    this->port = port;
}

int ListenDirective::getPort() const
{
    return port;
}