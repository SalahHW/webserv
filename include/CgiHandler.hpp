#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <iostream>
#include <cstdlib>
#include "ParseRequest.hpp"
#include "Server.hpp"
#include <algorithm>
#include <utility>
#include <vector>

class CgiHandler
{
	public:
		CgiHandler();
		~CgiHandler();
		CgiHandler (const CgiHandler &other);
		void printHeader();
		const std::string genQueryString();
		const std::string genRequestMethod();
		const std::string genServerProtocol();
	private:
		RequestParsed request;
};

#endif /* CGI_HANDLER_HPP */
