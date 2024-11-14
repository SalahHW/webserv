#pragma once

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <string>

namespace utils {

// File utils
void openFile(const std::string& fileName, std::ifstream& file);

// String utils
std::string trimWhitespace(const std::string& str);
std::string removeComments(const std::string& line);
std::string removeBraces(const std::string& line);

// Numbers utils
bool isIntCompatible(const std::string& str);
}