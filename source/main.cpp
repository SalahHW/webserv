#include <iostream>
#include <string>

#include "ConfigFile.hpp"
#include "Port.hpp"
#include "ServerManager.hpp"

int main(int argc, char** argv)
{
  std::string configFilePath("./config/webserv.conf");
  if (argc > 2)
  {
    std::cerr << "Usage = ./webserv [config file path]" << std::endl;
    exit(EXIT_FAILURE);
  }
  if (argc == 2)
    configFilePath = argv[1];

  ConfigFile server_configuration(configFilePath);
  if (!server_configuration.good())
  {
    std::cerr << "Error: Invalid configuration file" << std::endl;
    return (1);
  }

  server_configuration.displayInfo();

  ServerManager serverManager(server_configuration.getPortMap());
  if (serverManager.good())
    // serverManager.start();
    return (0);
}