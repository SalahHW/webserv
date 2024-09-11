/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 02:13:05 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/10 15:54:57 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
	string configFilePath("./config/webserv.conf");
	if (argc > 2)
	{
		cerr << "Usage = ./webserv [config file path]" << endl;
		return (1);
	}
	if (argc == 2)
		configFilePath = argv[1];

	// cout << configFilePath << endl;
	try
	{
		ConfigFile server_configuration(configFilePath);
	}
	catch (runtime_error &error)
	{
		cerr << error.what() << endl;
		return (1);
	}
	return (0);
}