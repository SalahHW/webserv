#include "EpollManagement.hpp"

EpollManagement::EpollManagement() {
    EpollInit();
}

EpollManagement::~EpollManagement() {

}

EpollManagement::EpollManagement(const EpollManagement& src) {

} 

EpollManagement& EpollManagement::operator=(const EpollManagement& src) {

}

void    EpollManagement::EpollInit(int listen_sock_fd) {
    this->epoll_fd = epoll_create1(0);
    if(epoll_fd == -1)
    {
        close(listen_sock_fd);
        throw EpollException("epoll_create1");
    }
}

void    EpollManagement::addListenerToEpoll(int listen_sock_fd) {
    this->event.events = EPOLLIN;
    event.data.fd = listen_sock_fd;
    if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, listen_sock_fd, &this->event) == -1)
    {
        close(listen_sockfd);
        throw EpollException("epoll_ctl"); 
    }
}