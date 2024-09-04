#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include    <stdexcept>
#include    <cstdlib>

class   SocketException : public std::runtime_error {
    public:
    explicit    SocketException(const std::string& excp)
        : std::runtime_error(excp) {
            //exit(EXIT_FAILURE);
        }
};

#endif