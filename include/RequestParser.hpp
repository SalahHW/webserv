#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <sstream>
#include <stdexcept>
#include <string>

#include "Request.hpp"

class RequestParser {
 public:
  void parse(const std::string &rawRequest, Request &request);
};

#endif  // REQUESTPARSER_HPP
