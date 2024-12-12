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
#include <sys/wait.h>
#include <cstring>
#include <unistd.h>

class CgiHandler
{
	public:
		CgiHandler();
		~CgiHandler();
		CgiHandler (const CgiHandler &other);

		const std::string convertSizetToString(size_t value);
		const std::string genContentLenght();
		const std::string genQueryString();
		const std::string genRequestMethod();
		const std::string genServerProtocol();
		const std::string &genScriptPath() const ;
		const std::string genPathInfo(const std::string &input);
		const std::string genHttpUserAgent();
		const std::string genHttpAccept();
		const std::string genHttpHost();
		const std::string genHttpAcceptLanguage();
		const std::string genHttpAcceptEncoding();
		const std::string genHttpConnexion();
		const std::string genHttpUpgradeInsecureRequests();
		const std::string genHttpSecFetchDest();
		const std::string genHttpSecFetchMode();
		const std::string genHttpSecFetchSite();
		const std::string genHttpPriority();

		std::pair<int, pid_t> cgiExecution();
		//////////FD,  pid_t
		void buildEnv();
		const std::vector<std::string>& getEnvVec() const;
		void printEnv(std::vector<std::string> &env);
		void setScriptPath(const std::string &originScriptPath);
		int	checkQueryStringPresence(std::string &uri);
	private:
		RequestParsed request;
		std::string scriptPath;
		std::vector<std::string> envVec;
};

#endif /*CGI_HANDLER_HPP*/
