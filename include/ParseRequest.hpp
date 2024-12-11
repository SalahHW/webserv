#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "Request.hpp"

class ParseRequest {
 public:
  ParseRequest(const std::string& request);
  ~ParseRequest();

  Request getParsedRequest() const;

 private:
  std::string request;
  Request parsedRequest;

  void parseRequestLine();
  void parseHeaders();
  void parseBody();
  std::string trim(const std::string& str) const;
};

#endif  // PARSEREQUEST_HPP
