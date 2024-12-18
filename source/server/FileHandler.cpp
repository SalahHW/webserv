#include "FileHandler.hpp"

#include <sys/stat.h>

FileHandler::FileHandler() {}
FileHandler::~FileHandler() {}

bool FileHandler::fileExists(const std::string& filePath) const {
  struct stat buffer;
  return (stat(filePath.c_str(), &buffer) == 0);
}

bool FileHandler::isAccessible(const std::string& filePath) const {
  std::ifstream file(filePath.c_str());
  return file.good();
}

std::string FileHandler::readFile(const std::string& filePath) const {
  std::ifstream file(filePath.c_str());
  if (!file) return "";

  std::ostringstream content;
  content << file.rdbuf();
  return content.str();
}
