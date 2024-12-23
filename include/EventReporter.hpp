#pragma once

#include <cerrno>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

class ServerManager;

class EventReporter {
public:
    ~EventReporter();
    EventReporter();

    bool good() const;

    bool addFD(int fd);
    void run(void (ServerManager::*eventHandler)(int fd, uint32_t events), ServerManager* instance);

private:
    bool isValid;
    int epollFd;

    void initializeEpoll();
    void closeEpoll();

    EventReporter(const EventReporter& other);
    EventReporter& operator=(const EventReporter& other);
};