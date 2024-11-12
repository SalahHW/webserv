/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderBuilder.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:44:25 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/07 13:44:45 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderBuilder.hpp"
#include <sstream>

HeaderBuilder::~HeaderBuilder() { }

HeaderBuilder::HeaderBuilder() { }

void HeaderBuilder::addHeader(const std::string& key, const std::string& value)
{
    headers[key] = value;
}

std::string HeaderBuilder::buildHeaders() const
{
    std::ostringstream headersStream;
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        headersStream << it->first << ": " << it->second << "\r\n";
    }
    return headersStream.str();
}
