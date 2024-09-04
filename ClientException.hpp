#ifndef CLIENTEXCEPTION_HPP
#define CLIENTEXCEPTION_HPP

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

class ClientException : public std::runtime_error {
public:
  explicit ClientException(const std::string &excp) : std::runtime_error(excp) {
    // exit(EXIT_FAILURE);
  }
};

#endif