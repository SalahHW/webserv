#ifndef LOCATIONMATCHER_HPP
#define LOCATIONMATCHER_HPP

#include <map>
#include <string>

#include "Location.hpp"

class LocationMatcher {
 public:
  LocationMatcher(const std::map<std::string, Location>& locs);

  const Location* findMatchingLocation(const std::string& requestPath) const;

 private:
  const std::map<std::string, Location>& locations;
};

#endif
