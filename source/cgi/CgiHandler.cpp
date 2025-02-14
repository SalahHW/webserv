/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvan-den <rvan-den@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:00:31 by rvan-den          #+#    #+#             */
/*   Updated: 2025/02/14 11:15:01 by rvan-den         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

CgiHandler::CgiHandler() {}

CgiHandler::CgiHandler(const Request &request)
{
    this->cgiExecution(request, request.getFd());
}

CgiHandler::~CgiHandler() {std::cout << "DESTRUCTION OF CGI" << std::endl;}

CgiHandler::CgiHandler(const CgiHandler &other) { (void)other; }

//void CgiHandler::cgiExecution(const Request &request, int outputFd)
//{
//    if (this->envVec.empty())
//        this->buildEnv(request);
//    std::vector<std::string> env = this->getEnvVec();
//    this->setCgiRetErrorCode(0);
//
//    const std::string scriptDirAss = "./var/www/cgi-bin";
//    const char *scriptDir = scriptDirAss.c_str();
//    const char *scriptName = this->extractScriptName(request.getUri());
//    char *const args[] = {const_cast<char *>(PY_INTERP),
//                          const_cast<char *>(scriptName), NULL};
//    char **envArray;
//    pid_t pid;
//    int status;
//    int pipefd[2];
//    int bodyPipefd[2];
//    size_t error_code = 0;
//
//    try
//    {
//        if (pipe(pipefd) < 0 || pipe(bodyPipefd) < 0)
//        {
//            throw std::runtime_error("Failed to create pipes");
//        }
//
//        envArray = this->allocateEnvArray(env);
//        pid = fork();
//        if (pid < 0)
//        {
//            throw std::runtime_error(FORK_ERR);
//        }
//        else if (pid == 0)
//        {
//            close(pipefd[0]);
//            if (dup2(pipefd[1], STDOUT_FILENO) < 0 || dup2(pipefd[1], STDERR_FILENO) < 0) {
//                error_code = 500;
//                throw std::runtime_error("Failed to redirect STDOUT/STDERR");
//            }
//            close(pipefd[1]);
//            close(bodyPipefd[1]);
//            if (dup2(bodyPipefd[0], STDIN_FILENO) < 0) {
//                error_code = 500;
//                throw std::runtime_error("Failed to redirect STDIN");
//            }
//            close(bodyPipefd[0]);
//            if (chdir(scriptDir) < 0) {
//                error_code = 500;
//                throw std::runtime_error(CGI_DIR_ERR);
//            }
//            if (execve(PY_INTERP, args, envArray) < 0) {
//                error_code = 500;
//                throw std::runtime_error(PY_INTERP);
//            }
//        }
//        else
//        {
//            close(bodyPipefd[0]);
//            if (!request.getBody().empty())
//            {
//                if (write(bodyPipefd[1], request.getBody().c_str(), request.getBody().size()) < 0) {
//                    error_code = 500;
//                    throw std::runtime_error("Failed to write request body to pipe");
//                }
//            }
//            close(bodyPipefd[1]);
//            close(pipefd[1]);
//
//            fd_set read_fds;
//            FD_ZERO(&read_fds);
//            FD_SET(pipefd[0], &read_fds);
//
//            struct timeval timeout;
//            timeout.tv_sec = 1;
//            timeout.tv_usec = 0;
//
//            int ret = select(pipefd[0] + 1, &read_fds, NULL, NULL, &timeout);
//            if (ret == -1)
//            {
//                error_code = 500;
//                throw std::runtime_error("select() failed");
//            }
//            else if (ret == 0)
//            {
//                error_code = 500;
//                kill(pid, SIGKILL);
//                waitpid(pid, &status, 0);
//                throw std::runtime_error("CGI script timeout");
//            }
//            else
//            {
//                char buffer[4096];
//                ssize_t bytesRead;
//                while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0)
//                {
//                    if (send(outputFd, buffer, bytesRead, MSG_NOSIGNAL) < 0) {
//                        throw std::runtime_error("Failed to send data to outputFd");
//                    }
//                }
//            }
//            close(pipefd[0]);
//            if (waitpid(pid, &status, 0) < 0) {
//                throw std::runtime_error(WAITPID_ERR);
//            }
//        }
//    }
//    catch (const std::exception &e)
//    {
//        if (error_code > 0) {
//            this->setCgiRetErrorCode(error_code);
//        }
//        std::cerr << "Error: " << e.what() << std::endl;
//        this->cleanupEnvArray(env, envArray);
//        return;
//    }
//    this->cleanupEnvArray(env, envArray);
//}

void CgiHandler::cgiExecution(const Request &request, int outputFd)
{
    if (this->envVec.empty())
        this->buildEnv(request);
    std::vector<std::string> env = this->getEnvVec();
    this->setCgiRetErrorCode(0);

    const std::string scriptDirAss = "./var/www/rgi-bin";
    const char *scriptDir = scriptDirAss.c_str();
    const char *scriptName = this->extractScriptName(request.getUri());
    char *const args[] = {const_cast<char *>(PY_INTERP),
                          const_cast<char *>(scriptName), NULL};
    char **envArray;
    pid_t pid;
    int status;
    int pipefd[2];
    int bodyPipefd[2];
    size_t error_code = 0;

    try
    {
        // Create pipes
        if (pipe(pipefd) < 0 || pipe(bodyPipefd) < 0)
        {
            if (pipe(pipefd) >= 0) {
                close(pipefd[0]);
                close(pipefd[1]);
            }
            throw std::runtime_error("Failed to create pipes");
        }

        // Allocate environment array
        envArray = this->allocateEnvArray(env);

        // Fork the process
        pid = fork();
        if (pid < 0)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            close(bodyPipefd[0]);
            close(bodyPipefd[1]);
            throw std::runtime_error(FORK_ERR);
        }
        else if (pid == 0)
        {
            close(pipefd[0]);
            if (dup2(pipefd[1], STDOUT_FILENO) < 0 || dup2(pipefd[1], STDERR_FILENO) < 0) {
                close(pipefd[1]);
                close(bodyPipefd[0]);
                close(bodyPipefd[1]);
                _exit(1);
            }
            close(pipefd[1]);

            close(bodyPipefd[1]);
            if (dup2(bodyPipefd[0], STDIN_FILENO) < 0) {
                close(bodyPipefd[0]);
                _exit(1); // Exit with error code
            }
            close(bodyPipefd[0]);

            if (chdir(scriptDir) < 0) {
                _exit(1); // Exit with error code
            }

            if (execve(PY_INTERP, args, envArray) < 0) {
                _exit(1); 
            }
        }
        else
        {
            close(bodyPipefd[0]); 
            if (!request.getBody().empty())
            {
                if (write(bodyPipefd[1], request.getBody().c_str(), request.getBody().size()) < 0) {
                    close(bodyPipefd[1]);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    error_code = 500;
                    throw std::runtime_error("Failed to write request body to pipe");
                }
            }
            close(bodyPipefd[1]);
            close(pipefd[1]);

            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(pipefd[0], &read_fds);

            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            int ret = select(pipefd[0] + 1, &read_fds, NULL, NULL, &timeout);
            if (ret == -1)
            {
                close(pipefd[0]);
                error_code = 500;
                throw std::runtime_error("select() failed");
            }
            else if (ret == 0)
            {
                close(pipefd[0]); 
                error_code = 500;
                kill(pid, SIGKILL);
                waitpid(pid, &status, 0);
                throw std::runtime_error("CGI script timeout");
            }
            else
            {
                char buffer[4096];
                ssize_t bytesRead;
                while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0)
                {
                    if (send(outputFd, buffer, bytesRead, MSG_NOSIGNAL) < 0) {
                        close(pipefd[0]);
                        throw std::runtime_error("Failed to send data to outputFd");
                    }
                }
                close(pipefd[0]);

                // Wait for the child process to exit
                if (waitpid(pid, &status, 0) < 0) {
                    throw std::runtime_error(WAITPID_ERR);
                }

                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    error_code = 500;
                    throw std::runtime_error("CGI script failed to execute");
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        if (error_code > 0) {
            this->setCgiRetErrorCode(error_code);
        }
        std::cerr << "Error: " << e.what() << std::endl;
        close(pipefd[0]);
        close(pipefd[1]);
        close(bodyPipefd[0]);
        close(bodyPipefd[1]);
        this->cleanupEnvArray(env, envArray);
        return;
    }
    this->cleanupEnvArray(env, envArray);
}

void CgiHandler::printEnv(std::vector<std::string> &env)
{
    std::vector<std::string>::iterator vec_it;
    for (vec_it = env.begin(); vec_it != env.end(); vec_it++)
        std::cout << "[DEBUG] : " << *vec_it << std::endl;
}