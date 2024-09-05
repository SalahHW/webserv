#ifndef EPOLLEXCEPTION_HPP
#define EPOLLEXCEPTION_HPP

#include <cstdlib>
#include <stdexcept>

class EpollException : public std::runtime_error {
public:
    explicit EpollException(const std::string& excp)
        : std::runtime_error(excp)
    {
        // exit(EXIT_FAILURE);
    }
};

#endif