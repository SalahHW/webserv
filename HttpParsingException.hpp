#ifndef HTTPPARSINGEXCEPTION_HPP
#define HTTPPARSINGEXCEPTION_HPP

#include <cstdlib>
#include <stdexcept>

class HttpParsingException : public std::runtime_error {
public:
    explicit HttpParsingException(const std::string& excp)
        : std::runtime_error(excp)
    {
    }
};

#endif
