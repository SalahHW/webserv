#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <map>

#include "Client.hpp"
#include "Request.hpp"
#include "RequestParser.hpp"
#include "RequestValidator.hpp"
#include "ResponseHandler.hpp"
#include "ResponseSender.hpp"

#ifndef CLIENT_TIMEOUT
#define CLIENT_TIMEOUT 60
#endif

class ClientManager {
 public:
  ClientManager();
  ~ClientManager();

  void addClient(int clientFd);
  void removeClient(int clientFd);
  void closeClient(int clientFd);
  void checkTimeouts();

  void handleClientRead(int clientFd);
  void handleClientWrite();

  void setRequestParser(RequestParser* rp) { requestParser = rp; }
  void setResponseHandler(ResponseHandler* rh) { responseHandler = rh; }
  void setResponseSender(ResponseSender* rs) { responseSender = rs; }
  void setRequestValidator(RequestValidator* rv) { requestValidator = rv; }

 private:
  std::map<int, Client> clients;
  RequestParser* requestParser;
  ResponseHandler* responseHandler;
  ResponseSender* responseSender;
  RequestValidator* requestValidator;
};

#endif
