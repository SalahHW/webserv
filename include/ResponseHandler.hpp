#pragma once

#include "HeaderBuilder.hpp"
#include "HttpStatusCode.hpp"
#include "Location.hpp"
#include "ParseRequest.hpp"
#include "Server.hpp"

class ResponseHandler {
public:
    ResponseHandler(RequestParsed& requestParsed, const Server& server);
    ~ResponseHandler();

    void handleResponse();
    std::string getResponse() const;

private:
    RequestParsed& requestParsed;
    const Server& server;
    std::string fullResponse;
};