#include "EpollManagement.hpp"

EpollManagement::EpollManagement() {

}

EpollManagement::~EpollManagement() {

}

EpollManagement::EpollManagement(const EpollManagement& src) {

} 

EpollManagement& EpollManagement::operator=(const EpollManagement& src) {

}

void    EpollManagement::addListenerToEpoll(int listen_sockfd) {
    this->event.events = EPOLLIN;
    event.data.fd = listen_sockfd;
    if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, listen_sockfd, &this->event) == -1)
    {
        close(listen_sockfd);
        throw EpollException("epoll_ctl"); 
    }
}