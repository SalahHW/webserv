#ifndef EPOLL_MANAGEMENT_HPP
#define EPOLL_MANAGEMENT_HPP

#include <unistd.h>
#include <sys/epoll.h>
#include "EpollException.hpp"

class EpollManagement {
    private:
    int epoll_fd;
    struct epoll_event event;

    public:
    EpollManagement();
    ~EpollManagement();
    EpollManagement(const EpollManagement& src);
    EpollManagement& operator=(const EpollManagement& src);
    void    addListenerToEpoll(int listen_sock_fd);
};

#endif