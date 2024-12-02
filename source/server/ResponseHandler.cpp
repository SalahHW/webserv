#include "ResponseHandler.hpp"

#include "HttpStatusCodeDeterminer.hpp"
#include "ResponseBuilder.hpp"

ResponseHandler::ResponseHandler(RequestParsed &requestParsed,
                                 const Server &server)
    : requestParsed(requestParsed), server(server) {}

ResponseHandler::~ResponseHandler() {}

void ResponseHandler::handleResponse() {
  ResponseBuilder responseBuilder(requestParsed, server);
  fullResponse = responseBuilder.buildResponse();
}

std::string ResponseHandler::getResponse() const { return fullResponse; }
