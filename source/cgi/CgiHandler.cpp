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

    // ! GET/POST -> content body (size/write) -> execve
    // try / catch for exec -> malloc protec + wcs handling
    // if POST request -> FD_IN body_html
    // handle delete[] on CgiHandler class destruction
    //  chier guele chemin physique parse dans requete / truc de salah ????? uri no during search
    // TODO : close all pipes

    // NEW TODO : adapt code to newly made request class, handle new cgi construction

#include "Cgi.hpp"

CgiHandler::CgiHandler() {
    this->setScriptPath("/home/sickest-one/Travail/webserv/www/cgi-bin/"); // modif to getCgiLocation (isCgiLocation ?)
	//request.method = "GET";
	//request.uri = "http://example.com/cgi-bin/cgi.py";
	//request.version = "HTTP/1.1";
}

CgiHandler::~CgiHandler()
{}

CgiHandler::CgiHandler(const CgiHandler &other) {
	(void)other;
}

std::pair<int, pid_t> CgiHandler::cgiExecution() {
    this->buildEnv();
    std::vector<std::string> env = this->getEnvVec();
    const char *scriptDir = this->genScriptPath().c_str();
    const char *scriptName = "cgi.py";
    char *const args[] = {const_cast<char *>(PY_INTERP), \
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
            if (this->checkQueryStringPresence(request.uri)) {
                throw std::runtime_error(QUERY_ERR);
            }
            if (chdir(scriptDir) < 0) {
                throw std::runtime_error(CGI_DIR_ERR);
            }
            // implement POST fd redirection
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
        return (std::make_pair(-1, pid));
    }
    this->cleanupEnvArray(env, envArray);
    return std::make_pair(status, pid);
}

//std::pair<int, pid_t> CgiHandler::cgiExecution() {
//    this->buildEnv();
//    std::vector<std::string> env = this->getEnvVec();
//    const char *scriptDir = this->genScriptPath().c_str();
//    const char *scriptName = "cgi.py";
//    char *const args[] = {const_cast<char *>(PY_INTERP), const_cast<char *>(scriptName), NULL};
//    char **envArray;
//    pid_t pid;
//    int status;
//
//    int inputPipe[2];  // Pipe for POST input
//    int outputPipe[2]; // Pipe for CGI output
//
//    try {
//        // Create pipes for input and output
//        if (pipe(inputPipe) < 0 || pipe(outputPipe) < 0) {
//            std::cerr << "Pipe creation error" << std::endl;
//            return std::make_pair(-1, -1);
//        }
//
//        envArray = this->allocateEnvArray(env);
//        pid = fork();
//        if (pid < 0) {
//            std::cerr << FORK_ERR << std::endl;
//            this->cleanupEnvArray(env, envArray);
//            return std::make_pair(-1, pid);
//        }
//        else if (pid == 0) {
//            // Child process
//            close(inputPipe[1]);  // Close unused write end of input pipe
//            close(outputPipe[0]); // Close unused read end of output pipe
//
//            // Handle POST method: Redirect STDIN to inputPipe[0]
//            if (request.method == "POST") {
//                if (dup2(inputPipe[0], STDIN_FILENO) < 0) {
//                    std::cerr << "Failed to redirect STDIN" << std::endl;
//                    std::exit(EXIT_FAILURE);
//                }
//            }
//            close(inputPipe[0]);
//
//            // Redirect STDOUT to outputPipe[1] for CGI output
//            if (dup2(outputPipe[1], STDOUT_FILENO) < 0) {
//                std::cerr << "Failed to redirect STDOUT" << std::endl;
//                std::exit(EXIT_FAILURE);
//            }
//            close(outputPipe[1]);
//
//            // Change working directory
//            if (chdir(scriptDir) < 0) {
//                std::cerr << CGI_DIR_ERR << std::endl;
//                std::exit(EXIT_FAILURE);
//            }
//
//            // Execute the CGI script
//            if (execve(PY_INTERP, args, envArray) < 0) {
//                std::cerr << EXECVE_ERR << std::endl;
//                std::exit(EXIT_FAILURE);
//            }
//        }
//        else {
//            // Parent process
//            close(inputPipe[0]);  // Close unused read end of input pipe
//            close(outputPipe[1]); // Close unused write end of output pipe
//
//            // Handle POST: Write request body to child's STDIN
//            if (request.method == "POST" && !requestBody.empty()) {
//                write(inputPipe[1], requestBody.c_str(), requestBody.size());
//            }
//            close(inputPipe[1]); // Close write end after sending the request body
//
//            // Read CGI output (child's STDOUT)
//            char buffer[1024];
//            std::string cgiOutput;
//            ssize_t bytesRead;
//            while ((bytesRead = read(outputPipe[0], buffer, sizeof(buffer) - 1)) > 0) {
//                buffer[bytesRead] = '\0';
//                cgiOutput += buffer;
//            }
//            close(outputPipe[0]);
//
//            // Wait for child process to finish
//            if (waitpid(pid, &status, 0) < 0) {
//                std::cerr << WAITPID_ERR << std::endl;
//                this->cleanupEnvArray(env, envArray);
//                return std::make_pair(-1, pid);
//            }
//
//            // Process CGI output (you may want to send this back to the client)
//            std::cout << "CGI Output:\n" << cgiOutput << std::endl;
//        }
//    }
//    catch (const std::exception &e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//        return std::make_pair(-1, pid);
//    }
//
//    this->cleanupEnvArray(env, envArray);
//    return std::make_pair(0, pid);
//}


void CgiHandler::printEnv(std::vector<std::string> &env)
{
    // FOR DEBUG, DELETE LATER
    std::vector<std::string>::iterator vec_it;
    for (vec_it = env.begin(); vec_it != env.end(); vec_it++) {
        std::cout << "[DEBUG] : " << *vec_it << std::endl;
    }
}