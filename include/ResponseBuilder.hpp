#ifndef RESPONSEBUILDER_HPP
#define RESPONSEBUILDER_HPP

#include <sstream>
#include <string>

class ResponseBuilder {
 public:
  std::string buildHeaders(int statusCode, size_t contentLength,
                           const std::string& contentType,
                           bool keepAlive) const;
};

#endif
