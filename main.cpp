#include "ServerIn.hpp"
#include "EpollManagement.hpp"
#include "ClientIn.hpp"

int main() {
    ServerIn Server;
    EpollManagement Epoll(Server.listen_sock_fd);
}