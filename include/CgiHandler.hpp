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
		const std::pair<std::string, std::string> getQueryString();
	private:
		RequestParsed request;
		void	testParse();
};

#endif /* CGI_HANDLER_HPP */
