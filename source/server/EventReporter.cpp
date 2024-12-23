#include "EventReporter.hpp"

EventReporter::~EventReporter()
{
    closeEpoll();
}

EventReporter::EventReporter()
    : isValid(true)
    , epollFd(-1)
{
    initializeEpoll();
}

bool EventReporter::good() const
{
    return this->isValid;
}

bool EventReporter::addFD(int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        std::cerr << "Error: Failed to add file descriptor \"" << fd << "\" to epoll";
        return false;
    }
    return true;
}

void EventReporter::run(void (ServerManager::*eventHandler)(int fd, uint32_t events), ServerManager* instance)
{
    struct epoll_event events[10];

    while (isValid) {
        int numEvents = epoll_wait(epollFd, events, 10, -1);

        if (numEvents == -1) {
            if (errno == EINTR) continue;
            std::cerr << "Error: epoll_wait failed" << std::endl;
            isValid = false;
            break;
        }

        for (int i = 0; i < numEvents; ++i) {
            (instance->*eventHandler)(events[i].data.fd, events[i].events);
        }
    }
}

void EventReporter::initializeEpoll()
{
    epollFd = epoll_create1(EPOLL_CLOEXEC);
    if (epollFd == -1) {
        std::cerr << "Error: Failed to initialize epoll" << std::endl;
        isValid = false;
    }
}

void EventReporter::closeEpoll()
{
    if (epollFd >= 0) {
        if (close(epollFd) == -1) {
            std::cerr << "Error: Failed to close epoll file descriptor" << std::endl;
        }
        epollFd = -1;
    }
}