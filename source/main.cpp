#include <iostream>
#include <string>

#include "ConfigFile.hpp"
#include "Port.hpp"
#include "ServerHandler.hpp"

int main(int argc, char** argv)
{
    std::string configFilePath("./config/webserv.conf");
    if (argc > 2) {
        std::cerr << "Usage = ./webserv [config file path]" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (argc == 2)
        configFilePath = argv[1];

    ConfigFile server_configuration(configFilePath);
    if (!server_configuration.good()) {
        std::cerr << "Error: Invalid configuration file" << std::endl;
        return (1);
    }

    //TODO: Update Configuration to setup  a default server for a port

    std::map<int, Port> ports = server_configuration.getPortMap();
    std::map<int, Port>::const_iterator itPort;

    for (itPort = ports.begin(); itPort != ports.end(); ++itPort) {
        itPort->second.displayHosts();
    }
    
    // ServerHandler serverHandler(ports);
    return (0);
}