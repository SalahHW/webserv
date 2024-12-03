#include <iostream>
#include <string>

#include "webserv.hpp"
#include "CgiHandler.hpp"

using namespace std;

string loadConfiguration(int argc, char **argv) {
  string configFilePath("./config/webserv.conf");
  if (argc > 2) throw runtime_error("Usage = ./webserv [config file path]");
  if (argc == 2) configFilePath = argv[1];
  return (configFilePath);
}

int main(int argc, char **argv) {
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
