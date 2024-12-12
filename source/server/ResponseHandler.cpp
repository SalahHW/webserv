#include "ResponseHandler.hpp"

#include "Client.hpp"

ResponseHandler::ResponseHandler(Client& client, const Server& server)
    : client(client), server(server) {
  std::cout << "[DEBUG] ResponseHandler initialized for client FD "
            << client.getClientFd() << std::endl;
}

ResponseHandler::~ResponseHandler() {}

void ResponseHandler::handleResponse(Request& request, int requestId) {
  ResponseBuilder responseBuilder(request, server);
  Response response = responseBuilder.buildResponse();

  client.getResponseMap()[requestId] = std::make_pair(request, response);
  client.enqueueResponse(response);

  if (!request.isKeepAlive()) {
    client.setConnectionShouldClose(true);
  }

  std::cout << "[DEBUG] Response for request ID " << requestId
            << " enqueued for client FD " << client.getClientFd() << std::endl;
}
