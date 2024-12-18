#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <fstream>
#include <sstream>
#include <string>

class FileHandler {
 public:
  FileHandler();
  ~FileHandler();

  bool fileExists(const std::string& filePath) const;
  bool isAccessible(const std::string& filePath) const;
  std::string readFile(const std::string& filePath) const;
};

#endif
