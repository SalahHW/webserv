#pragma once

#include <sys/epoll.h>
#include <map>
#include <unistd.h>
#include <iostream>

class FunctorBase {
public:
    virtual void operator()(uint32_t events) = 0;
};

class Epoll {
public:
    Epoll();
    ~Epoll();

    void add(int fd, uint32_t events, FunctorBase* handler);
    void del(int fd);
    void mod(int fd, uint32_t events);
    int wait(struct epoll_event *events, int maxevents, int timeout);
private:
    int epollfd;
    std::map<int, FunctorBase* > eventHandler;
};