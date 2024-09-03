#ifndef EPOLL_MANAGEMENT_HPP
#define EPOLL_MANAGEMENT_HPP

#include <unistd.h>
#include <sys/epoll.h>
#include "EpollException.hpp"

#define MAX_EVENTS 10

class EpollManagement {
    private:
    int epoll_fd;
    struct epoll_event events[MAX_EVENTS];
    int nb_events;

    public:
    EpollManagement(int listen_sock_fd);
    ~EpollManagement();
    EpollManagement(const EpollManagement& src);
    EpollManagement& operator=(const EpollManagement& src);
    void    addListenerToEpoll(int listen_sock_fd);
    int     getEpollFd();
    int     getNbEvents();
    struct epoll_event  getEventsStruct();
};

#endif