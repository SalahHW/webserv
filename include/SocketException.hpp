#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <cstdlib>
#include <stdexcept>

class SocketException : public std::runtime_error {
 public:
  explicit SocketException(const std::string& excp)
      : std::runtime_error(excp) {}
};

#endif