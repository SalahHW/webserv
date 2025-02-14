#pragma once

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <string>

namespace utils
{

  // File utils
  void openFile(const std::string& fileName, std::ifstream& file);

  // String utils
  std::string trimWhitespace(const std::string& str);
  std::string removeComments(const std::string& line);
  std::string removeBraces(const std::string& line);
  void toLowerCase(std::string &str);

  // Return true if the string can be converted to an integer
  bool isIntCompatible(const std::string& str);
  // Convert a string into a long value. Return false if the conversion can't be done
  bool convertToLong(const char* str, long& value);
  // Convert a string to int. Return false if the conversion can't be done
  bool convertToInt(const char* str, int& value);
  // Return the product of a and b in result. Return false if the product can't be
  // stored in a long
  bool safeMultiplyLong(long a, long b, long& result);
  bool safeMultiplyInt(int a, int b, int& result);
} // namespace utils