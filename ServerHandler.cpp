#include "ServerHandler.hpp"

ServerHandler::ServerHandler()
{
}

ServerHandler::~ServerHandler()
{
}

void ServerHandler::addServer(int serverFd, const Server& src)
{
    this->serverList.insert(std::make_pair(serverFd, src));
}