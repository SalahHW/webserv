#include "EpollManagement.hpp"

EpollManagement::EpollManagement(int listen_sock_fd) {
    try {
        EpollInit();
        addToEpoll(listen_sock_fd);
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

EpollManagement::EpollManagement(const EpollManagement& src) : epoll_fd(src.epoll_fd), nb_events(src.nb_events) {
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

void    EpollManagement::EpollInit() {
    this->epoll_fd = epoll_create1(0);
    if(this->epoll_fd == -1) {
        throw EpollException("epoll_create1");
    }
}

void    EpollManagement::addToEpoll(int sock_fd) {
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sock_fd;
    if(epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, sock_fd, &event) == -1) {
        throw EpollException("epoll_ctl"); 
    }
}

void    EpollManagement::startToListen(int listen_sock_fd) {
    struct epoll_event events[MAX_EVENTS];
    std::cout << "----------STARTING TO LISTENING----------" << std::endl;
    while(1) {
        std::cout << "epoll fd = " << this->epoll_fd << std::endl;
        this->nb_events = epoll_wait(this->epoll_fd, events, MAX_EVENTS, -1);
        if(this->nb_events == -1) {
            throw EpollException("epoll_wait " + std::string(strerror(errno)));
        }
        for (int i = 0; i < this->nb_events; i++) {
            if (events[i].data.fd == listen_sock_fd) {
                std::cout << i << std::endl;
                std::cout << "listen fd = " << listen_sock_fd << std::endl;
                std::cout << "current fd = " << events[i].data.fd << std::endl;
                ClientIn client(listen_sock_fd, *this);
                std::cout << "Connected" << std::endl;
            }
            else {
                std::cout << "hadoubilah"<< std::endl;
                handleClientData(this->epoll_fd, events[i].data.fd);
            }
        }
    }   
}

void    EpollManagement::handleClientData(int epoll_fd, int client_fd) {
    char buffer[1024];
    ssize_t count = recv(client_fd, buffer, sizeof(buffer), 0);
    if (count == -1) {
        epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
        throw EpollException("recv");
    }
    else if (count == 0) {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
        throw EpollException("Client disconnected");
    }
    else {
        buffer[count] = '\0';
        std::cout << "Request: " << buffer << std::endl;
    }
}