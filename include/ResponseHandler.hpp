#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include <map>
#include <string>

#include "FileHandler.hpp"
#include "LocationMatcher.hpp"
#include "Response.hpp"

class ResponseHandler {
 public:
  ResponseHandler(const std::map<int, std::string>& errorPages,
                  const std::map<std::string, Location>& locations);

  Response handleResponse(const std::string& method,
                          const std::string& requestPath, int bodySize,
                          bool keepAlive);

 private:
  const std::map<int, std::string>& errorPages;
  LocationMatcher locationMatcher;
  FileHandler fileHandler;

  int determineStatusCode(const std::string& method,
                          const std::string& filePath, int bodySize) const;
  std::string getErrorPage(int statusCode) const;
  std::string findErrorPage(int statusCode) const;
  std::string buildFullPath(const Location& location,
                            const std::string& requestPath) const;
  std::string getReasonPhrase(int statusCode) const;
  std::string intToString(int value) const;
};

#endif
