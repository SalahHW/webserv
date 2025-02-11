#pragma once

#include "DeleteRequestHandler.hpp"
#include "ErrorRequestHandler.hpp"
#include "GetRequestHandler.hpp"
#include "PostRequestHandler.hpp"
#include "Request.hpp"
#include <string>

class RequestRouter
{
  public:
  static std::string route(const Request& request);
};