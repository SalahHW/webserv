#pragma once

#include "Request.hpp"
#include <string>

class DeleteRequestHandler
{
  public:
  static std::string handle(const Request& request);
};