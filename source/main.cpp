#include <iostream>
#include <string>

#include "ConfigExtractor.hpp"
#include "ConfigFile.hpp"
// #include "Port.hpp"
#include "ServerHandler.hpp"

std::string loadConfiguration(int argc, char** argv)
{
    std::string configFilePath("./config/webserv.conf");
    if (argc > 2)
        throw std::runtime_error("Usage = ./webserv [config file path]");
    if (argc == 2)
        configFilePath = argv[1];
    return (configFilePath);
}

int main(int argc, char** argv)
{
    std::string configFilePath = loadConfiguration(argc, argv);
    ConfigFile server_configuration(configFilePath);
    if (!server_configuration.good()) {
        std::cerr << "Error: Invalid configuration file" << std::endl;
        return (1);
    }

    std::vector<Server> servers = ConfigExtractor::extractServers(server_configuration);
    // Configfinalizer verifie que tout les ports sont OK et finalise la configuration
    // Le premier serveur est le serveur par defaut pour ce port sauf si defaut_server est specifier dans un autre serveur
    // ConfigFinalizer ==> Liste de port
    ConfigFinalizer configFinalizer;
    configFinalizer.finalizeConfig(servers);
    std::map<int, Port> ports = configFinalizer.getPorts();

    // ServerHandler serverHandler(ports);
    return (0);
}