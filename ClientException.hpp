#ifndef CLIENTEXCEPTION_HPP
#define CLIENTEXCEPTION_HPP

#include <stdexcept>

class   ClientException : public std::runtime_error {
    public:
    explicit    ClientException(const std::string& excp)
        : std::runtime_error(excp) {}  
};

#endif