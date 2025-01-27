/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-den <rvan-den@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:00:31 by rvan-den          #+#    #+#             */
/*   Updated: 2025/01/27 15:22:19 by rvan-den         ###   ########.fr       */
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

    // ! GET/POST -> content body (size/write) -> execve
    // try / catch for exec -> malloc protec + wcs handling
    // if POST request -> FD_IN body_html
    // handle delete[] on CgiHandler class destruction
    //  chier guele chemin physique parse dans requete / truc de salah ????? uri no during search
    // TODO : close all pipes

    // NEW TODO : adapt code to newly made request class, handle new cgi construction

#include "Cgi.hpp"

CgiHandler::CgiHandler() {
    //this->setScriptPath("/home/sickest-one/Travail/webserv/www/cgi-bin/"); // modif to getCgiLocation (isCgiLocation ?)
	//request.method = "GET";
	//request.uri = "http://example.com/cgi-bin/cgi.py";
	//request.version = "HTTP/1.1";
}

CgiHandler::CgiHandler(Request &request, const std::string &scriptLoc, const std::string &content) {
    this->cgiExecution(request, scriptLoc, content);
}

CgiHandler::~CgiHandler()
{}

CgiHandler::CgiHandler(const CgiHandler &other) {
	(void)other;
}

void CgiHandler::cgiExecution(Request &request, const std::string &scriptLoc, const std::string &content) {
    (void)content;
    this->buildEnv(request);
    std::vector<std::string> env = this->getEnvVec();
    //const char *scriptDir = this->genScriptPath().c_str();
    const char *scriptDir = scriptLoc.c_str();
    const char *scriptName = "cgi.py";
    char *const args[] = {const_cast<char *>(PY_INTERP), 
    const_cast<char *>(scriptName), NULL};
    char **envArray;
    pid_t pid;
    int status;

    try
    {
        envArray = this->allocateEnvArray(env);
        pid = fork();
        if (pid < 0) {
            throw std::runtime_error(FORK_ERR);
        }
        else if (pid == 0) {
            if (chdir(scriptDir) < 0) {
                throw std::runtime_error(CGI_DIR_ERR);
            }
            if (execve(PY_INTERP, args, envArray) < 0) {
                throw std::runtime_error(PY_INTERP);
            }
        }
        else {
            if (waitpid(pid, &status, 0) < 0) {
                throw std::runtime_error(WAITPID_ERR);
            }
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Error : " << e.what() << std::endl;
        std::cout << "READY TO CLEAN" << std::endl;
        this->cleanupEnvArray(env, envArray);
        return ;
    }
    this->cleanupEnvArray(env, envArray);
    return ;
}

void CgiHandler::printEnv(std::vector<std::string> &env)
{
    // FOR DEBUG, DELETE LATER
    std::vector<std::string>::iterator vec_it;
    for (vec_it = env.begin(); vec_it != env.end(); vec_it++) {
        std::cout << "[DEBUG] : " << *vec_it << std::endl;
    }
}