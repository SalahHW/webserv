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

  if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
  {
    std::cerr << "Error: Failed to add file descriptor \"" << fd << "\" to epoll";
    return false;
  }
  return true;
}

int EventReporter::getNextEvent(uint32_t& eventFlag)
{
  struct epoll_event events[1];
  int numEvents = epoll_wait(epollFd, events, 1, 0);

  if (numEvents < 0 || numEvents == 0)
  {
    if (errno == EINTR)
    {
      std::cerr << "Error: epoll_wait failed" << std::endl;
      isValid = false;
    }
    return -1;
  }

  eventFlag = events[0].events;
  return events[0].data.fd;
}

void EventReporter::initializeEpoll()
{
  epollFd = epoll_create1(EPOLL_CLOEXEC);
  if (epollFd == -1)
  {
    std::cerr << "Error: Failed to initialize epoll" << std::endl;
    isValid = false;
  }
}

void EventReporter::closeEpoll()
{
  if (epollFd >= 0)
  {
    if (close(epollFd) == -1)
    {
      std::cerr << "Error: Failed to close epoll file descriptor" << std::endl;
    }
    epollFd = -1;
  }
}