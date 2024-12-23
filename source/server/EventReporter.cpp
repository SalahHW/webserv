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

void EventReporter::initializeEpoll()
{
    // TODO: check if EPOLL_CLOEXEX don't close fd's in the main process
    epollFd = epoll_create1(EPOLL_CLOEXEC);
    if (epollFd == -1) {
        std::cerr << "Error: Epoll initialization failed" << std::endl;
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