#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <iostream>
#include <cstdlib>
#include "ParseRequest.hpp"
#include "Server.hpp"

class CgiHandler
{
	public:
		CgiHandler(const Server &server);
		~CgiHandler();
		CgiHandler (const CgiHandler &other);
	private:
		const Server &receivedServer;
		void	testParse();
};

#endif /* CGI_HANDLER_HPP */
