#pragma once

#include <stdexcept>

class HttpException : public std::runtime_error {
 public:
  HttpException(size_t code, const std::string& msg)
      : std::runtime_error(msg), statusCode(code) {}

  size_t getCode() const { return statusCode; }

 private:
  size_t statusCode;
};