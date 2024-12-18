#ifndef REQUESTVALIDATOR_HPP
#define REQUESTVALIDATOR_HPP

#include <cstdlib>

#include "Request.hpp"
#include "Response.hpp"

class RequestValidator {
 public:
  bool validate(const Request &request, Response &response);
};

#endif
