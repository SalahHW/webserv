/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 02:13:05 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/12 02:17:38 by sbouheni         ###   ########.fr       */
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
    try {
        string configFilePath = loadConfiguration(argc, argv);
        ConfigFile server_configuration(configFilePath);
    } catch (runtime_error& error) {
        cerr << error.what() << endl;
        return (1);
    }
    return (0);
}