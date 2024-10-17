/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:20:42 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/16 09:10:41 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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