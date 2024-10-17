/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 02:13:05 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/17 17:03:14 by sbouheni         ###   ########.fr       */
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
    if (!server_configuration.good())
        return (1);
    ServerHandler serverHandler(server_configuration);
    serverHandler.displayServerHandlerInfo();
    return (0);
}