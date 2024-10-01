#include "ServerHandler.hpp"

ServerHandler::ServerHandler()
{
}

ServerHandler::~ServerHandler()
{
}

void ServerHandler::addServer(const Server& src)
{
    this->serverList.push_back(src);
}