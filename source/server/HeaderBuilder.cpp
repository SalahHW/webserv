/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderBuilder.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:44:25 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/13 16:41:13 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderBuilder.hpp"
#include <sstream>

HeaderBuilder::~HeaderBuilder() { }

HeaderBuilder::HeaderBuilder()
    : statusCode(Constants::DEFAULT_STATUS_CODE)
    , reasonPhrase(Constants::DEFAULT_REASON_PHRASE)
{
}

void HeaderBuilder::setStatusCode(int code)
{
    statusCode = code;
}

void HeaderBuilder::setReasonPhrase(const std::string& phrase)
{
    reasonPhrase = phrase;
}

void HeaderBuilder::addHeader(const std::string& key, const std::string& value)
{
    headers[key] = value;
}

std::string HeaderBuilder::buildStatusLine() const
{
    std::ostringstream statusLineStream;
    statusLineStream << Constants::HTTP_VERSION << " " << statusCode << " "
                     << reasonPhrase << Constants::LINE_TERMINATOR;
    return statusLineStream.str();
}

std::string HeaderBuilder::buildHeaders() const
{
    std::ostringstream headersStream;
    std::map<std::string, std::string>::const_iterator it = headers.begin();
    for (; it != headers.end(); ++it) {
        headersStream << it->first << Constants::HEADER_SEPARATOR
                      << it->second << Constants::LINE_TERMINATOR;
    }
    return headersStream.str();
}
