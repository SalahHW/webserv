#pragma once

#include <string>

#include "HeaderBuilder.hpp"
#include "Location.hpp"
#include "ParseRequest.hpp"
#include "Server.hpp"

class ResponseBuilder {
 public:
  ResponseBuilder(RequestParsed& requestParsed, const Server& server);
  ~ResponseBuilder();

  std::string buildResponse();

 private:
  RequestParsed& requestParsed;
  const Server& server;

  HeaderBuilder headerBuilder;
  std::string body;
  Location matchingLocation;

  void prepareResponse();
  void prepareSuccessResponse();
  void prepareRedirectionResponse();
  void prepareClientErrorResponse();
  void prepareServerErrorResponse();

  bool findMatchingLocation(const std::string& uri, Location& matchingLocation);
  void handleDirectoryRequest(const std::string& filePath);
  void serveFile(const std::string& filePath);
  bool isDirectory(const std::string& path) const;
  std::string generateDirectoryListing(const std::string& directoryPath,
                                       const std::string& uri) const;
  std::string readFile(const std::string& filePath) const;
  std::string getContentType(const std::string& filePath) const;
  std::string generateDefaultErrorPage(int statusCode) const;
  bool fileExists(const std::string& filePath) const;
};
