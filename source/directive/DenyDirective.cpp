#include "DenyDirective.hpp"
#include "Block.hpp"

DenyDirective::~DenyDirective() { }

DenyDirective::DenyDirective(Block* currentContext, const std::string& fullDirectiveLine)
	: Directive(currentContext, fullDirectiveLine)
{
	setName("deny");
	setMinArgs(1);
	setMaxArgs(1);
	addContext("location");
	//TODO: Look for valid contexts for deny
	addContext("limit_except");
	validate();
}

DenyDirective::DenyDirective(const DenyDirective& other)
	: Directive(other)
{
}

DenyDirective& DenyDirective::operator=(const DenyDirective& other)
{
	if (this != &other) {
		Directive::operator=(other);
	}
	return *this;
}

bool DenyDirective::validateSpecific()
{
	if (arguments[0] != "all")
	{
		std::cerr << "Error: Bad argument in deny directive inside \"" << currentContext->getName() << "\" block." << std::endl;
		return false;
	}
	std::vector<std::string> contextAgruments = currentContext->getArguments();
	for (size_t i = 0; i < contextAgruments.size(); i++)
	{
		if (contextAgruments[i] == "GET")
			this->getAccepted = false;
		else if (contextAgruments[i] == "POST")
			this->postAccepted = false;
		else if (contextAgruments[i] == "DELETE")
			this->postAccepted = false;
	}
	return true;
}

void DenyDirective::displayInfo() const
{
	std::cout << "Name : " << this->getName() << std::endl;
}

void DenyDirective::apply(Location &location) {
	location.setGetAccepted(this->getAccepted);
	location.setPostAccepted(this->postAccepted);
	location.setDeleteAccepted(this->deleteAccepted);
}