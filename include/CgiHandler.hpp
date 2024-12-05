#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <iostream>
#include <cstdlib>
#include "ParseRequest.hpp"
#include "Server.hpp"
#include <algorithm>
#include <utility>
#include <vector>
#include <string>

class CgiHandler
{
	public:
		CgiHandler();
		~CgiHandler();
		CgiHandler (const CgiHandler &other);

		const std::string genContentLenght();
		const std::string genQueryString();
		const std::string genRequestMethod();
		const std::string genServerProtocol();
		const std::string genScriptPath();
		const std::string genPathInfo(const std::string &input);
		const std::string convertSizetToString(size_t value);

		std::vector<std::string> buildEnv();
		void printEnv(std::vector<std::string> &env);
	private:
		RequestParsed request;
};

#endif /*CGI_HANDLER_HPP*/
