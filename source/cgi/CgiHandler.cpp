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

// need to chdir into script dir before exec
// in child proc (QUERY STR PARSE -> if '?querystring=4' -> error !!!!)
// modify scriptPath to python interpreter path
// return (the pair you made)
// ! GET/POST -> content body (size/write) -> execve
// try / catch for exec -> malloc protec + wcs handling
// if POST request -> FD_IN body_html
// handle delete[] on CgiHandler class destruction
//  chier guele chemin physique parse dans requete / truc de salah ????? uri no during search
std::pair<int, pid_t> CgiHandler::cgiExecution() {
    this->buildEnv();
    std::vector<std::string> env = this->getEnvVec();
    char **envArray = new char *[env.size() + 1];
    envArray[env.size()] = NULL;

    const char *pythonInterpreter = "/usr/bin/python3";
    this->setScriptPath("/home/sickest_one/Travail/webserv/www/cgi-bin/cgi.py");
    const char *scriptPath = this->genScriptPath().c_str();
    char *const args[] = {const_cast<char *>(pythonInterpreter), const_cast<char *>(scriptPath), NULL};

    for (size_t i = 0; i < env.size(); i++) {
        envArray[i] = new char[env[i].size() + 1];
        std::strcpy(envArray[i], env[i].c_str());
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::perror("pipe");
        for (size_t i = 0; i < env.size(); ++i)
            delete[] envArray[i];
        delete[] envArray;
        return std::make_pair(-1, -1);
    }
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Error: Fork failed." << std::endl;
        close(pipefd[0]);
        close(pipefd[1]);
        for (size_t i = 0; i < env.size(); ++i)
            delete[] envArray[i];
        delete[] envArray;
        return std::make_pair(-1, -1);
    }
    else if (pid == 0) {
        try {
            std::string scriptDir = "/home/sickest_one/Travail/webserv/www/cgi-bin";
            if (chdir(scriptDir.c_str()) == -1)
            {
                std::perror("chdir");
                std::exit(EXIT_FAILURE);
            }
            if (this->checkQueryStringPresence(request.uri)) {
                std::cerr << "Error : Invalid QUERY_STRING" << std::endl;
                // close fd's
            }
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            if (request.method == "POST") {
                std::string contentBody = this->getContentBody(); // need to implement
                int bodyPipe[2];
                if (pipe(bodyPipe) == -1)
                {
                    std::perror("pipe");
                    std::exit(EXIT_FAILURE);
                }
                write(bodyPipe[1], contentBody.c_str(), contentBody.size());
                close(bodyPipe[1]);
                dup2(bodyPipe[0], STDIN_FILENO);
                close(bodyPipe[0]);
            }
            if (execve(pythonInterpreter, args, envArray) == -1) {
                std::perror("execve");
                std::exit(EXIT_FAILURE);
            }
        }
        catch (const std::exception &e) {
            std::cerr << "Exception in child process: " << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    else {
        close(pipefd[1]);
        for (size_t i = 0; i < env.size(); ++i)
            delete[] envArray[i];
        delete[] envArray;
        return std::make_pair(pipefd[0], pid);
    }
    for (size_t i = 0; i < env.size(); ++i) {
        delete[] envArray[i];
    }
    delete[] envArray;
    return std::make_pair(-1, -1); // Should not reach here
}


void CgiHandler::printEnv(std::vector<std::string> &env)
{
    // FOR DEBUG, DELETE LATER
    std::vector<std::string>::iterator vec_it;
    for (vec_it = env.begin(); vec_it != env.end(); vec_it++) {
        std::cout << "[DEBUG] : " << *vec_it << std::endl;
    }
}
