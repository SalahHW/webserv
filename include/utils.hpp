/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:20:42 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/02 09:43:10 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <climits>

namespace utils {
	
// File utils
void openFile(const std::string& fileName, std::ifstream& file);

// String utils
std::string trimWhitespace(const std::string& str);
std::string removeComments(const std::string& line);

// Numbers utils
bool isIntCompatible(const string& str);
}