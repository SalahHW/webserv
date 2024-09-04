#ifndef EPOLL_MANAGEMENT_HPP
#define EPOLL_MANAGEMENT_HPP

#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>
#include "EpollException.hpp"
#include "ClientIn.hpp"

#define MAX_EVENTS 10

class ClientIn;

class EpollManagement {
    private:
    int epoll_fd;
    int nb_events;

    public:
    EpollManagement(int listen_sock_fd);
    ~EpollManagement();
    EpollManagement(const EpollManagement& src);
    EpollManagement& operator=(const EpollManagement& src);
    void    addToEpoll(int listen_sock_fd);
    int     getEpollFd();
    int     getNbEvents();
    void    EpollInit();
    void    startToListen(int listen_sock_fd);
    void    handleClientData(int epoll_fd, int client_fd);
};

#endif