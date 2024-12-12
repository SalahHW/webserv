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

    //AUTH_TYPE
    //CONTENT_LENGTH
    //CONTENT_TYPE
    //GATEWAY_INTERFACE
    //HTTP_*
    //PATH_INFO
    //PATH_TRANSLATED
    //QUERY_STRING
    //REMOTE_ADDR
    //REMOTE_HOST
    //REMOTE_IDENT
    //REMOTE_USER
    //REQUEST_METHOD
    //SCRIPT_NAME
    //SERVER_NAME
    //SERVER_PORT
    //SERVER_PROTOCOL
    //SERVER_SOFTWARE

    //  [virtual path][extra path information]?[query string]

#include "CgiHandler.hpp"

CgiHandler::CgiHandler() {
	request.method = "GET";
	request.uri = "http://example.com/cgi-bin/script.cgi?num1=2&num2=4";
	request.version = "HTTP/1.1";
}

CgiHandler::~CgiHandler()
{}

CgiHandler::CgiHandler(const CgiHandler &other) {
	(void)other;
}

void CgiHandler::cgiExecution()
{
    // will need to start exec this with a try / catch for GET (or POST, dont remember..)
    std::vector<std::string> env = this->buildEnv();
    char **envArray = new char*[env.size() + 1];
    envArray[env.size()] = NULL;
    this->setScriptPath("/home/sickest_one/Travail/webserv/www/cgi-bin/cgi.py");
    const char *scriptPath = this->genScriptPath().c_str();
    std::cout << "[DEBUG] : " << scriptPath << std::endl;
    char *const args[] = {const_cast<char *>(scriptPath), NULL};

    for (size_t i = 0; i < env.size(); i++)
    {
        envArray[i] = new char[env[i].size() + 1];
        std::strcpy(envArray[i], env[i].c_str());
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Error: Fork failed." << std::endl;
    } else if (pid == 0) {
        if (execve(scriptPath, args, envArray) == -1) {
            std::perror("execve");
            _exit(EXIT_FAILURE);
        }
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            std::perror("waitpid");
        } else if (WIFEXITED(status)) {
            std::cout << "CGI script exited with status: " << WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
            std::cerr << "CGI script was terminated by signal: " << WTERMSIG(status) << std::endl;
        }
    }


    for (std::size_t i = 0; i < env.size(); i++)
        delete[] envArray[i];
    delete[] envArray;

}

void CgiHandler::printEnv(std::vector<std::string> &env)
{
    // FOR DEBUG, DELETE LATER
    std::vector<std::string>::iterator vec_it;
    for (vec_it = env.begin(); vec_it != env.end(); vec_it++) {
        std::cout << "[DEBUG] : " << *vec_it << std::endl;
    }
}
