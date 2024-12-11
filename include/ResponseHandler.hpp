#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include <iostream>

#include "Request.hpp"
#include "Response.hpp"
#include "ResponseBuilder.hpp"

class ResponseHandler {
 public:
  ResponseHandler(Client& client, const Server& server);
  ~ResponseHandler();

  void handleResponse(Request& request, int requestId);

 private:
  Client& client;
  const Server& server;
};

#endif  // RESPONSEHANDLER_HPP
#