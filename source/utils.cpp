/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:22:18 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/10 16:28:31 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void utils::openFile(const std::string& fileName, std::ifstream& file)
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