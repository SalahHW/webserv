#include "EpollManagement.hpp"

EpollManagement::EpollManagement(int listen_sock_fd) {
    try {
        EpollInit(listen_sock_fd);
        addListenerToEpoll(listen_sock_fd);
        startToListen(listen_sock_fd);
    }
    catch(const EpollException& excp) {
        std::cerr << "Epoll error: " << excp.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

EpollManagement::~EpollManagement() {
    close(this->epoll_fd);
}

EpollManagement::EpollManagement(const EpollManagement& src) {

} 

EpollManagement& EpollManagement::operator=(const EpollManagement& src) {

}

int EpollManagement::getEpollFd() {
    return this->epoll_fd;
}

int EpollManagement::getNbEvents() {
    return this->nb_events;
}

struct epoll_event EpollManagement::getEventStruct() {
    return this->event;
}

void    EpollManagement::EpollInit(int listen_sock_fd) {
    this->epoll_fd = epoll_create1(0);
    if(this->epoll_fd == -1) {
        throw EpollException("epoll_create1");
    }
}

void    EpollManagement::addListenerToEpoll(int listen_sock_fd) {
    this->event.events = EPOLLIN;
    this->event.data.fd = listen_sock_fd;
    if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, listen_sock_fd, &this->event) == -1) {
        throw EpollException("epoll_ctl"); 
    }
}


void    EpollManagement::startToListen(int listen_sock_fd) {
    struct epoll_event events[MAX_EVENTS];
    while(1) {
        this->nb_events = epoll_wait(this->epoll_fd, events, MAX_EVENTS, -1);
        if(this->nb_events == -1) {
            throw EpollException("epoll_wait");
        }
        for (int i = 0; i < this->nb_events; i++) {
            if (events[i].data.fd == listen_sock_fd) {
                ClientIn client(listen_sock_fd, this);
            }
            //else {
            //    handleClientData();
           // }
        }
    }   
}