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
    int getNextEvent(uint32_t &eventFlag);

private:
    bool isValid;
    int epollFd;

    void initializeEpoll();
    void closeEpoll();

    EventReporter(const EventReporter& other);
    EventReporter& operator=(const EventReporter& other);
};