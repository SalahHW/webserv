/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 02:13:05 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/05 11:37:22 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <iostream>
#include <string>

using namespace std;

string loadConfiguration(int argc, char** argv)
{
    string configFilePath("./config/webserv.conf");
    if (argc > 2)
        throw runtime_error("Usage = ./webserv [config file path]");
    if (argc == 2)
        configFilePath = argv[1];
    return (configFilePath);
}

int main(int argc, char** argv)
{
    string configFilePath = loadConfiguration(argc, argv);
    ConfigFile server_configuration(configFilePath);
    if (!server_configuration.good()) {
        cerr << "Error: Invalid configuration file" << endl;
        return (1);
    }
    ServerHandler serverHandler(server_configuration);
    serverHandler.displayServerHandlerInfo();
    return (0);
}