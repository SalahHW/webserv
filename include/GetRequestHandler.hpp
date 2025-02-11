#pragma once

#include "Request.hpp"
#include <string>

class GetRequestHandler
{
  public:
  static std::string handle(const Request& request);
};