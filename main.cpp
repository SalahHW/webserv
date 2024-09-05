#include "ClientIn.hpp"
#include "EpollManagement.hpp"
#include "ServerIn.hpp"

int main()
{
    ServerIn Server;
    EpollManagement Epoll(Server.getListenSockFd());
}