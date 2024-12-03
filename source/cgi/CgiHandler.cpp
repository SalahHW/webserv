/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sickest_one <sickest_one@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:00:31 by rvan-den          #+#    #+#             */
/*   Updated: 2024/12/03 16:05:04 by sickest_one      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler()
{
	request.method = "GET";
	request.uri = "http://example.com/cgi-bin/script.cgi?num1=2&num2=4";
	request.version = "HTTP/1.1";
}

CgiHandler::~CgiHandler()
{}

CgiHandler::CgiHandler(const CgiHandler &other)
{
	(void)other;
}

void	CgiHandler::testParse()
{}

const std::pair<std::string, std::string> CgiHandler::getQueryString()
{
    std::string::const_iterator it = std::find(request.uri.begin(), request.uri.end(), '?');
    if (it != request.uri.end()) {
        size_t pos = it - request.uri.begin();
        std::string beforeQuery = request.uri.substr(0, pos);
        std::string query = request.uri.substr(pos + 1);
		std::pair<std::string, std::string> queryPair(beforeQuery, query);
        return (queryPair);
    }
    return std::make_pair(request.uri, "");
}

// TO DO : BUILD ENV ARRAY, K.I.S.S
