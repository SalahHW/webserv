#include "HttpStatusCodeDeterminer.hpp"

#include <sys/stat.h>

#include <cstring>
#include <iostream>
#include <map>
#include <vector>

// Trouver la location correspondante au URI
bool HttpStatusCodeDeterminer::findMatchingLocation(
    const std::string& uri, Location& matchingLocation) {
  const std::map<std::string, Location>& locations = server.getLocations();
  size_t longestMatch = 0;
  bool found = false;

  for (std::map<std::string, Location>::const_iterator it = locations.begin();
       it != locations.end(); ++it) {
    const std::string& locationPath = it->first;
    if (uri.find(locationPath) == 0 && locationPath.length() > longestMatch) {
      longestMatch = locationPath.length();
      matchingLocation = it->second;
      found = true;
    }
  }
  return found;
}

// Vérifier si la méthode est autorisée pour une location donnée
bool HttpStatusCodeDeterminer::isMethodAllowed(const Location& location,
                                               const std::string& method) {
  if (method == "GET") return location.getGetAccepted();
  if (method == "POST") return location.getPostAccepted();
  if (method == "DELETE") return location.getDeleteAccepted();
  return false;  // Méthode non reconnue
}

// Déterminer le chemin du fichier correspondant à la location et au URI
std::string HttpStatusCodeDeterminer::determineFilePath(
    const Location& location, const std::string& uri) {
  std::string root = location.getRootDirectory();
  std::string relativePath = uri.substr(location.getPath().length());
  if (relativePath.empty() || relativePath == "/") {
    relativePath = location.getIndexFile();
  }
  return normalizePath(root + "/" + relativePath);
}

// Vérifier si un fichier existe
bool HttpStatusCodeDeterminer::fileExists(const std::string& filePath) const {
  struct stat buffer;
  return (stat(filePath.c_str(), &buffer) == 0);
}

// Vérifier les permissions d'accès
bool HttpStatusCodeDeterminer::hasAccessPermissions(
    const std::string& filePath) const {
  struct stat buffer;
  if (stat(filePath.c_str(), &buffer) == 0) {
    return (buffer.st_mode & S_IRUSR) !=
           0;  // Vérifier les permissions de lecture
  }
  return false;
}

// Vérifier si un type MIME est supporté
bool HttpStatusCodeDeterminer::isSupportedMediaType(
    const std::string& mediaType) const {
  static const char* supportedTypes[] = {"text/html", "text/plain", "image/png",
                                         "image/jpeg", "application/json"};
  for (size_t i = 0; i < sizeof(supportedTypes) / sizeof(supportedTypes[0]);
       ++i) {
    if (mediaType == supportedTypes[i]) {
      return true;
    }
  }
  return false;
}

// Vérifier si une plage demandée est valide
bool HttpStatusCodeDeterminer::isValidRange(const Request& request,
                                            unsigned long fileSize) const {
  std::map<std::string, std::string>::const_iterator it =
      request.getHeaders().find("Range");
  if (it != request.getHeaders().end()) {
    const std::string& rangeHeader = it->second;
    if (rangeHeader.find("bytes=") == 0) {
      std::string range = rangeHeader.substr(6);
      size_t dash = range.find('-');
      if (dash != std::string::npos) {
        unsigned long start =
            std::strtoul(range.substr(0, dash).c_str(), NULL, 10);
        unsigned long end =
            (dash + 1 < range.size())
                ? std::strtoul(range.substr(dash + 1).c_str(), NULL, 10)
                : fileSize - 1;
        return start < fileSize && end < fileSize && start <= end;
      }
    }
  }
  return false;  // Plage invalide ou absente
}

// Normaliser un chemin (supprimer les "//")
std::string HttpStatusCodeDeterminer::normalizePath(
    const std::string& path) const {
  std::string normalizedPath = path;
  size_t pos;
  while ((pos = normalizedPath.find("//")) != std::string::npos) {
    normalizedPath.replace(pos, 2, "/");
  }
  return normalizedPath;
}
