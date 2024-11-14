#pragma once

#include "Client.hpp"
#include "HeaderBuilder.hpp"
#include "HttpRequest.hpp"
#include <string>

class Client;

class ResponseHandler {
public:
    ResponseHandler(Client& client, const RequestParsed& requestParsed);
    ~ResponseHandler();

    void handleResponse();

private:
    Client& client;
    const RequestParsed& requestParsed;
    HeaderBuilder headerBuilder;
    std::string body;
    int statusCode;
    std::string reasonPhrase;

    void buildResponse();
    void handleSuccess();
    void handleError();
    void loadErrorPage();
    void sendResponse();
    void closeConnectionIfNeeded();
    std::string getReasonPhrase(int code) const;
};