/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:22:18 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/16 09:16:52 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

using namespace std;

void utils::openFile(const string& fileName, ifstream& file)
{
    file.open(fileName.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + fileName);
    }
}

string utils::trimWhitespace(const string& str)
{
    string::size_type first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == string::npos)
        return "";
    string::size_type last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

string utils::removeComments(const string& line)
{
    size_t commentPos = line.find('#');
    if (commentPos != string::npos)
        return (line.substr(0, commentPos));
    return (line);
}

string utils::removeBraces(const string& line)
{
    size_t openBracePos = line.find('{');
    if (openBracePos != string::npos)
        return (line.substr(0, openBracePos));
    return (line);
}

bool utils::isIntCompatible(const string& str)
{
    if (str.empty()) {
        return false;
    }
    size_t start = (str[0] == '-') ? 1 : 0;
    if (str.find_first_not_of("0123456789", start) != std::string::npos) {
        return false;
    }
    errno = 0;
    char* endptr;
    long value = std::strtol(str.c_str(), &endptr, 10);

    if (errno == ERANGE || *endptr != '\0' || value > INT_MAX || value < INT_MIN) {
        return false;
    }
    return true;
}

bool utils::convertToLong(const char* str, long& value)
{
    if (!str || !*str)
        return false;

    errno = 0;
    char* endptr;

    value = strtol(str, &endptr, 10);

    if (errno == ERANGE || endptr == str || *endptr != '\0')
        return false;
    return true;
}

bool utils::safeMultiplyLong(long a, long b, long& result)
{
    if (a > 0 && b > 0 && a > LONG_MAX / b) {
        return false;
    }
    if (a > 0 && b < 0 && b < LONG_MIN / a) {
        return false;
    }
    if (a < 0 && b > 0 && a < LONG_MIN / b) {
        return false;
    }
    if (a < 0 && b < 0 && a < LONG_MAX / b) {
        return false;
    }

    result = a * b;
    return true;
}