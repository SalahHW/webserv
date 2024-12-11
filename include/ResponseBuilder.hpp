#ifndef RESPONSEBUILDER_HPP
#define RESPONSEBUILDER_HPP

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Location.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"

class ResponseBuilder {
 public:
  ResponseBuilder(Request& request, const Server& server);
  ~ResponseBuilder();

  Response buildResponse();

 private:
  void prepareSuccessResponse();
  void prepareRedirectionResponse();
  void prepareClientErrorResponse();
  void prepareServerErrorResponse();
  bool findMatchingLocation(const std::string& uri, Location& matchingLocation);
  bool isDirectory(const std::string& path) const;
  std::string generateDirectoryListing(const std::string& directoryPath,
                                       const std::string& uri) const;
  std::string readFile(const std::string& filePath) const;
  std::string getContentType(const std::string& filePath) const;
  std::string generateDefaultErrorPage(int statusCode) const;
  bool fileExists(const std::string& filePath) const;
  std::string normalizePath(const std::string& path) const;

  Request& request;
  const Server& server;
  Response response;
};

#endif  // RESPONSEBUILDER_HPP
