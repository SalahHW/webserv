#include "EpollManagement.hpp"

EpollManagement::EpollManagement(int listen_sock_fd) {
    try {
        EpollInit();
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

EpollManagement::EpollManagement(const EpollManagement& src) : epoll_fd(src.epoll_fd), event(src.event), nb_events(src.nb_events) {
} 

EpollManagement& EpollManagement::operator=(const EpollManagement& src) {
    this->epoll_fd = src.epoll_fd;
    return *this;
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

void    EpollManagement::EpollInit() {
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

void    EpollManagement::addClientToEpoll(int client_sock_fd) {
    this->event.events = EPOLLIN;
    this->event.data.fd = client_sock_fd;
    if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, client_sock_fd, &this->event) == -1) {
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
                ClientIn client(listen_sock_fd, *this);
                std::cout << "Connected" << std::endl;
            }
            //else {
            //    handleClientData();
           // }
        }
    }   
}