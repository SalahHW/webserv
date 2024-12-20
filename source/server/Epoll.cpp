#include "Epoll.hpp"

Epoll::Epoll() {
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        std::cerr << "Error: Failed to create epoll file descriptor" << std::endl;
    }
}

Epoll::~Epoll() {
    close(epollfd);
    for (std::map<int, FunctorBase*>::iterator it = eventHandler.begin(); it != eventHandlers.end(); ++it) {
        delete it->second;
    }
    eventHandler.clear();
}

void Epoll::add(int fdToAdd, uint32_t events, FunctorBase* handler) {
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fdToAdd;
    if(epoll_ctl(this->epollfd, EPOLL_CTL_ADD, fdToAdd, &event) == -1) {
        std::cerr << "Error: Failed to add file descriptor to epoll" << std::endl;
    }
    eventHandler[fdToAdd] = handler;
}

void Epoll::del(int fd) {
    if(epoll_ctl(this->epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        std::cerr << "Error: Failed to delete file descriptor from epoll" << std::endl;
    }
    if(eventHandler.find(fd) != eventHandler.end()) {
        delete eventHandler[fd];
        eventHandler.erase(fd);
    }
}

void Epoll::mod(int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;

    if(epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev) == -1) {
        std::cerr << "Error: Failed to modify file descriptor in epoll" << std::endl;
    }
}

int Epoll::wait(struct epoll_event *events, int maxevents, int timeout) {
    int nfds = epoll_wait(epollfd, events, maxevents, timeout);
    if(nfds == -1) {
        std::cerr << "Error: Failed to wait for events" << std::endl;
    }
    for (int i = 0; i < nfds; ++i)  {
        int fd = events[i].data.fd;
        uint32_t ev = events[i].events;
        if(eventHandler.find(fd) != eventHandler.end()) {
        (*eventHandler[fd])(ev);
        }
    }
}