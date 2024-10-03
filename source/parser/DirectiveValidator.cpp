/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveValidator.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:47:28 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/03 13:01:01 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DirectiveValidator.hpp"

using namespace std;

DirectiveValidator::~DirectiveValidator()
{
}

DirectiveValidator::DirectiveValidator() {
	validationFunctions["listen"] = &DirectiveValidator::validateListen;
	validationFunctions["server_name"] = &DirectiveValidator::validateServerName;
	validationFunctions["client_max_body_size"] = &DirectiveValidator::validateClientMaxBodySize;
	validationFunctions["error_page"] = &DirectiveValidator::validateErrorPage;
}

bool DirectiveValidator::validate(const Directive& directive) {
	map<string, ValidationFunction>::const_iterator it = validationFunctions.find(directive.getName());
	if (it == validationFunctions.end()) {
		cerr << "Unknown directive: " << directive.getName() << endl;
		return false;
	}
	return (this->*(it->second))(directive);
}

bool DirectiveValidator::validateListen(const Directive& directive){
	if (directive.getArguments().size() != 1) {
		cerr << "Invalid number of arguments for listen directive" << endl;
		return false;
	}
	if (!utils::isIntCompatible(directive.getArguments()[0]) || (atoi(directive.getArguments()[0].c_str()) < 0 || atoi(directive.getArguments()[0].c_str()) > 65535)) {
		cerr << "Invalid port number: " << directive.getArguments()[0] << endl;
		return false;
	}
	cout << "Valid listen directive: " << directive.getArguments()[0] << endl;
	return true;
}

bool DirectiveValidator::validateServerName(const Directive& directive){
	if (directive.getArguments().size() != 1) {
		cerr << "Invalid number of arguments for server_name directive" << endl;
		return false;
	}
	return true;
}

bool DirectiveValidator::validateClientMaxBodySize(const Directive& directive){
	if (directive.getArguments().size() != 1) {
		cerr << "Invalid number of arguments for client_max_body_size directive" << endl;
		return false;
	}
	if (!isValidSize(directive.getArguments()[0])) {
		cerr << "Invalid size: " << directive.getArguments()[0] << endl;
		return false;
	}
	return true;
}

bool DirectiveValidator::validateErrorPage(const Directive& directive){
	if (directive.getArguments().size() < 2) {
		cerr << "Invalid number of arguments for error_page directive" << endl;
		return false;
	}
	return true;
}

bool DirectiveValidator::validateRoot(const Directive& directive){
	if (directive.getArguments().size() != 1) {
		cerr << "Invalid number of arguments for root directive" << endl;
		return false;
	}
	return true;
}

bool DirectiveValidator::isValidSize(const string& str) const {
	if (str.empty())
		return false;
	
	size_t pos = 0;
	while (pos < str.size() && isdigit(str[pos]))
		pos++;
	if (pos < str.size())
	{
		char unit = tolower(str[pos]);
		if (unit == 'k' || unit == 'm' || unit == 'g')
		{
			cout << "Valid size: " << str << endl;
			return pos == str.size() - 1;
		}
		else
		{
			cerr << "Invalid unit: " << unit << endl;
			return false;
		}
	}
	cout << "Valid size: " << str << endl;
	return true;
}