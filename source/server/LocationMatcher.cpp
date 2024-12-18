#include "LocationMatcher.hpp"

LocationMatcher::LocationMatcher(const std::map<std::string, Location>& locs)
    : locations(locs) {}

const Location* LocationMatcher::findMatchingLocation(
    const std::string& requestPath) const {
  const Location* bestMatch = 0;
  size_t bestMatchLength = 0;

  for (std::map<std::string, Location>::const_iterator it = locations.begin();
       it != locations.end(); ++it) {
    const std::string& locationPath = it->first;

    if (requestPath.find(locationPath) == 0) {
      size_t currentLength = locationPath.size();

      if (currentLength > bestMatchLength) {
        bestMatch = &(it->second);
        bestMatchLength = currentLength;
      }
    }
  }

  return bestMatch;
}
