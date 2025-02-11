#pragma once

#include "Request.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <string>

class Request;

class RequestParser
{
  public:
  static bool parse(const std::string& rawRequest, Request& request);

  private:
  ~RequestParser();
  RequestParser();

  static bool parseRequestLine(std::stringstream& requestStream, Request& request);
  static bool parseHeader(std::stringstream& requestStream, Request& request);
  static void parseBody(std::stringstream& requestStream, Request& request);
};