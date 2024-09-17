/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:20:42 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/12 01:53:16 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <string>

namespace utils {
void openFile(const std::string& fileName, std::ifstream& file);
std::string trimWhitespace(const std::string& str);
std::string removeComments(const std::string& line);
}