#ifndef CGI_HPP
#define CGI_HPP

#define PY_INTERP "/usr/bin/python3"
#define CGI_DIR_ERR "Error : Could not open cgi directory"
#define FORK_ERR "Error : Fork() failed"
#define EXECVE_ERR "Error : execve shat himself"
#define WAITPID_ERR "Error : waitpid"
#define QUERY_ERR "Error : Tryed to give query string in child process"
#define ENVARRAY_MALLOC_FAIL "Error : Memory allocation for envArray failed."
#define CGI_TIMEOUT_MS 500

#include <sys/time.h>
#include <sys/select.h>
#include <csignal>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cerrno>
#include <string>
#include <utility>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include "Request.hpp"
#include "ResponseBuilder.hpp"

class CgiHandler {
 public:
  CgiHandler();
  CgiHandler(const Request &request);
  ~CgiHandler();
  CgiHandler(const CgiHandler &other);

  const std::string convertSizetToString(size_t value);
  const char *extractScriptName(const std::string &path);
  const std::string genContentLenght(const std::string &contentLenght);
  const std::string genQueryString(const std::string &queryString);
  const std::string genRequestMethod(const std::string &requestMethod);
  const std::string genServerProtocol(const std::string &serverProtocol);
  const std::string &genScriptPath() const;
  const std::string genPathInfo(const std::string &input);
  const std::string genHttpUserAgent(const std::string &httpUserAgent);
  const std::string genHttpAccept(const std::string &httpAccept);
  const std::string genHttpHost(const std::string &httpHost);
  const std::string genHttpAcceptLanguage(const std::string &acceptLanguage);
  const std::string genHttpAcceptEncoding(const std::string &acceptEncoding);
  const std::string genHttpConnexion(const std::string &httpConnexion);
  const std::string genHttpUpgradeInsecureRequests(
      const std::string &httpUpgradeInsecureRequests);
  const std::string genHttpSecFetchDest(const std::string &httpSecFetchDest);
  const std::string genHttpSecFetchMode(const std::string &htppSecFetchMode);
  const std::string genHttpSecFetchSite(const std::string &httpSecFetchSite);
  const std::string genHttpPriority(const std::string &httpPriority);

  void cgiExecution(const Request &request, int outputFd);
  //////////FD,  pid_t
  void buildEnv(const Request &requestForEnv);
  char **allocateEnvArray(const std::vector<std::string> &env);
  void cleanupEnvArray(const std::vector<std::string> &env, char **envArray);
  void setCgiRetErrorCode(size_t error_code);
  size_t  getCgiRetErrorCode();
  const std::vector<std::string> &getEnvVec() const;
  void printEnv(std::vector<std::string> &env);
  void setScriptPath(const std::string &originScriptPath);
  int checkQueryStringPresence(std::string &uri);

 private:
  // RequestParsed request;
  std::string scriptPath;
  std::vector<std::string> envVec;
  int error_code;
};

#endif /*CGI_HPP*/