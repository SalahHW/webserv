#pragma once

#include <string>
#include <map>
#include "Location.hpp"

class VirtualHost {
    public:
        VirtualHost();
        ~VirtualHost();
        VirtualHost(const VirtualHost &src);
        VirtualHost &operator=(const VirtualHost &src);

        void setClientMaxBodySize(int clientMaxBodySize);
        void setErrorPage(const std::map<int, std::string>& errorPages);
        void setLocation(const std::map<std::string, Location>& locations);
        
        int getClientMaxBodySize() const;
        const std::map<int, std::string>& getErrorPages() const;
        const std::map<std::string, Location>& getLocations() const;

    private:
        int clientMaxBodySize;
        std::map<int, std::string> errorPages;
        std::map<std::string, Location> locations;
};