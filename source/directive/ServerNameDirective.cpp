#include "ServerNameDirective.hpp"

ServerNameDirective::~ServerNameDirective() { }

ServerNameDirective::ServerNameDirective(const std::string& currentContext, const std::string& fullDirectiveLine)
    : Directive(currentContext, fullDirectiveLine)
    , serverName("default")
{
    setName("server_name");
    setMinArgs(1);
    setMaxArgs(1);
    addContext("server");
	validate();
}

ServerNameDirective::ServerNameDirective(const ServerNameDirective& other)
    : Directive(other)
    , serverName(other.serverName)
{
}

ServerNameDirective& ServerNameDirective::operator=(const ServerNameDirective& other)
{
    if (this != &other) {
        Directive::operator=(other);
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

void ServerNameDirective::displayInfo() const
{
    std::cout << "Name : " << this->getName() << std::endl << "- server name : " << serverName << std::endl;
}

void ServerNameDirective::apply(Server& server)
{
    server.setName(serverName);
}

void ServerNameDirective::setServerName(const std::string& serverName)
{
	this->serverName = serverName;
}

const std::string& ServerNameDirective::getServername() const
{
	return serverName;
}