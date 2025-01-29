/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:00:31 by rvan-den          #+#    #+#             */
/*   Updated: 2025/01/29 14:25:44 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

CgiHandler::CgiHandler() {}

CgiHandler::CgiHandler(const Request &request)
{
    this->cgiExecution(request, request.getFd());
}

CgiHandler::~CgiHandler() {}

CgiHandler::CgiHandler(const CgiHandler &other) { (void)other; }

void CgiHandler::cgiExecution(const Request &request, int outputFd)
{
    if (this->envVec.empty())
        this->buildEnv(request);
    std::vector<std::string> env = this->getEnvVec();
    this->printEnv(env);
    std::cout << "[DEBUG] : BODY SIZE = " << request.getBody().size() << std::endl;

    const std::string scriptDirAss = "/home/sickest_one/Travail/webserv/var/www/cgi-bin";
    const char *scriptDir =
        scriptDirAss
            .c_str(); // supposed to have the full path via location (salah ?)
    const char *scriptName = this->extractScriptName(request.getUri());
    char *const args[] = {const_cast<char *>(PY_INTERP),
                          const_cast<char *>(scriptName), NULL};
    char **envArray;
    pid_t pid;
    int status;
    int pipefd[2];

    try
    {
        // Create a pipe for interprocess communication
        if (pipe(pipefd) < 0)
        {
            throw std::runtime_error("Failed to create pipe");
        }

        envArray = this->allocateEnvArray(env);
        pid = fork();
        if (pid < 0)
        {
            throw std::runtime_error(FORK_ERR);
        }
        else if (pid == 0)
        {
            // Child process: redirect STDOUT to the pipe
            close(pipefd[0]); // Close read end
            if (dup2(pipefd[1], STDOUT_FILENO) < 0)
            {
                throw std::runtime_error("Failed to redirect STDOUT");
            }
            if (dup2(pipefd[1], STDERR_FILENO) <
                0)
            { // Optional: Redirect STDERR as well
                throw std::runtime_error("Failed to redirect STDERR");
            }
            close(pipefd[1]); // Close write end

            // Change directory and execute the script
            if (chdir(scriptDir) < 0)
            {
                throw std::runtime_error(CGI_DIR_ERR);
            }
            if (execve(PY_INTERP, args, envArray) < 0)
            {
                throw std::runtime_error(PY_INTERP);
            }
        }
        else
        {
            // Parent process: read from the pipe and send output
            close(pipefd[1]); // Close write end

            char buffer[1024];
            ssize_t bytesRead;
            while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0)
            {
                if (send(outputFd, buffer, bytesRead, MSG_NOSIGNAL) < 0)
                {
                    throw std::runtime_error("Failed to send data to outputFd");
                }
            }
            close(pipefd[0]); // Close read end after reading
            // Wait for the child process to finish
            if (waitpid(pid, &status, 0) < 0)
            {
                throw std::runtime_error(WAITPID_ERR);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        this->cleanupEnvArray(env, envArray);
        return;
    }

    this->cleanupEnvArray(env, envArray);
}

void CgiHandler::printEnv(std::vector<std::string> &env)
{
    // FOR DEBUG, DELETE LATER
    std::vector<std::string>::iterator vec_it;
    for (vec_it = env.begin(); vec_it != env.end(); vec_it++)
    {
        std::cout << "[DEBUG] : " << *vec_it << std::endl;
    }
}