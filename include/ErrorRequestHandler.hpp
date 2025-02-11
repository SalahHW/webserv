#pragma once

#include <sstream>
#include <string>

class ErrorRequestHandler
{
  public:
  static std::string generateErrorResponse(int errorCode);
};