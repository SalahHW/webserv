#pragma once

#include "Request.hpp"
#include <string>

class PostRequestHandler
{
  public:
  static std::string handle(const Request& request);
};