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

void ClientMaxBodySizeDirective::setMaxBodySize(size_t maxBodySize)
{
	this->maxBodySize = maxBodySize;
}

size_t ClientMaxBodySizeDirective::getMaxBodySize() const
{
	return maxBodySize;
}