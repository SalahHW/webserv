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

    // need to chdir into script dir before exec
    // in child proc (QUERY STR PARSE -> if '?querystring=4' -> error !!!!)
    // modify scriptPath to python interpreter path
    // return (the pair you made)
    // ! GET/POST -> content body (size/write) -> execve
    // try / catch for exec -> malloc protec + wcs handling
    // if POST request -> FD_IN body_html
    // handle delete[] on CgiHandler class destruction
    //  chier guele chemin physique parse dans requete / truc de salah ????? uri no during search
    // TODO : close all pipes

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

std::pair<int, pid_t> CgiHandler::cgiExecution()
{
    this->buildEnv();
    std::vector<std::string> env = this->getEnvVec();
    char **envArray = new char *[env.size() + 1];
    envArray[env.size()] = NULL;

    // Modify scriptPath to Python interpreter path
    const char *pythonInterpreter = "/usr/bin/python3";
    this->setScriptPath("/home/sickest_one/Travail/webserv/www/cgi-bin/cgi.py");
    const char *scriptPath = this->genScriptPath().c_str();
    char *const args[] = {const_cast<char *>(pythonInterpreter), const_cast<char *>(scriptPath), NULL};

    for (size_t i = 0; i < env.size(); i++)
    {
        envArray[i] = new char[env[i].size() + 1];
        std::strcpy(envArray[i], env[i].c_str());
    }

    for (size_t i = 0; i < env.size(); ++i)
        delete[] envArray[i];
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
