#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Request.hpp"
#include "Response.hpp"

class RequestHandler {
 public:
  RequestHandler();
  Response handleRequest(const Request &request);

 private:
  Response handleGet(const Request &request);
  Response handlePost(const Request &request);
  Response handleDelete(const Request &request);

  Response buildErrorResponse(int statusCode, const std::string &reason);
  bool fileExists(const std::string &filePath) const;
  std::string readFile(const std::string &filePath) const;
  void writeFile(const std::string &filePath, const std::string &data) const;
  bool deleteFile(const std::string &filePath) const;
  std::string intToString(int number) const;
};

#endif
