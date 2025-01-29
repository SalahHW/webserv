#pragma once

#include <map>
#include <string>

#include "Location.hpp"

class VirtualHost {
 public:
  ~VirtualHost();
  VirtualHost();
  VirtualHost(const VirtualHost& src);
  VirtualHost& operator=(const VirtualHost& src);

  void setName(const std::string& name);
  void setClientBodyTempPath(const std::string& path);
  void setClientMaxBodySize(int clientMaxBodySize);
  void setErrorPages(const std::map<size_t, std::string>& errorPages);
  void setLocations(const std::map<std::string, Location>& locations);

  const std::string& getName() const;
  const std::string& getClientBodyTempPath() const;
  int getClientMaxBodySize() const;
  const std::map<size_t, std::string>& getErrorPages() const;
  const std::map<std::string, Location>& getLocations() const;

 private:
  std::string name;
  std::string clientBodyTempPath;
  int clientMaxBodySize;
  std::map<size_t, std::string> errorPages;
  std::map<std::string, Location> locations;
};