#pragma once

#include "Location.hpp"
#include <map>
#include <string>

class VirtualHost {
public:
    ~VirtualHost();
    VirtualHost();
    VirtualHost(const VirtualHost& src);
    VirtualHost& operator=(const VirtualHost& src);

    void setName(const std::string& name);
    void setClientMaxBodySize(int clientMaxBodySize);
    void setErrorPages(const std::map<int, std::string>& errorPages);
    void setLocations(const std::map<std::string, Location>& locations);

    const std::string& getName() const;
    int getClientMaxBodySize() const;
    const std::map<int, std::string>& getErrorPages() const;
    const std::map<std::string, Location>& getLocations() const;

private:
    std::string name;
    int clientMaxBodySize;
    std::map<int, std::string> errorPages;
    std::map<std::string, Location> locations;
};