/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenDirective.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:16:08 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 17:26:46 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListenDirective.hpp"

ListenDirective::~ListenDirective() { }

ListenDirective::ListenDirective(const std::string& currentContext, const std::string& fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine)
    , port(0)
{
    setName("listen");
    setMinArgs(1);
    setMaxArgs(1);
    addContext("server");
    validate();
}

ListenDirective::ListenDirective(const ListenDirective& other)
    : Directive(other)
    , port(other.port)
{
}

ListenDirective& ListenDirective::operator=(const ListenDirective& other)
{
    if (this != &other) {
        Directive::operator=(other);
        port = other.port;
    }
    return *this;
}

bool ListenDirective::validateSpecific()
{
    if (!utils::isIntCompatible(getArguments()[0])) {
        std::cerr << "Error: Directive 'listen' in \"" << getCurrentContext() << "\" has an invalid port number." << std::endl;
        return false;
    }
    this->port = atoi(getArguments()[0].c_str());
    if (port < 0 || port > 65535) {
        std::cerr << "Error: Directive 'listen' in \"" << getCurrentContext() << "\" has a port number out of range. (0 - 65535)" << std::endl;
        return false;
    }
    return true;
}

void ListenDirective::displayInfo() const
{
    std::cout << "Name : " << this->getName() << std::endl << "- port : " << port << std::endl;
}

void ListenDirective::apply(Server& server)
{
    server.setPort(port);
}

void ListenDirective::setPort(int port)
{
    this->port = port;
}

int ListenDirective::getPort() const
{
    return port;
}