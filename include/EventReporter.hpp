#pragma once

#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

class EventReporter {
public:
    ~EventReporter();
    EventReporter();

    bool good() const;

    bool addFD(int fd);

private:
    bool isValid;
    int epollFd;

    void initializeEpoll();
    void closeEpoll();

    EventReporter(const EventReporter& other);
    EventReporter& operator=(const EventReporter& other);
};