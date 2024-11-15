#pragma once

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

    void prepareResponse();

    void prepareSuccessResponse();
    void prepareRedirectionResponse();
    void prepareClientErrorResponse();
    void prepareServerErrorResponse();

    // Utility methods
    bool findMatchingLocation(const std::string& uri, Location& matchingLocation);
    bool isDirectory(const std::string& path);
    std::string generateDirectoryListing(const std::string& directoryPath, const std::string& uri);
    std::string readFile(const std::string& filePath);
    std::string getContentType(const std::string& filePath);
    std::string getReasonPhrase(int code);

    // Additional members
    Location matchingLocation;
};
