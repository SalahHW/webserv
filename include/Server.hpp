#ifndef SERVER_HPP
#define SERVER_HPP

#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <map>
#include <string>

#include "Location.hpp"

class Server {
 public:
  Server();
  ~Server();
  Server(const Server &src);
  Server &operator=(const Server &src);

  void setPort(int port);
  void setClientMaxBodySize(int size);
  void setName(const std::string &name);
  void setErrorPages(const std::map<int, std::string> &errorPages);
  void addErrorPage(int errorCode, const std::string &errorPath);
  void addLocation(const Location &location);

  int getListenFd() const;
  int getPort() const;
  int getClientMaxBodySize() const;
  const std::string &getName() const;
  const std::map<int, std::string> &getErrorPages() const;
  const std::map<std::string, Location> &getLocations() const;

  void paramFd();
  void displayServerInfo() const;

 private:
  void setListenFd();
  void bindSocket() const;
  void setToListen() const;
  void makeSocketNonBlocking() const;

  int listenFd;
  int port;
  std::string name;
  int clientMaxBodySize;
  std::map<int, std::string> errorPages;
  std::map<std::string, Location> locations;
  struct sockaddr_in addr;
};

#endif
