#pragma once

#include "Location.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Block;

class Directive
{
  public:
  virtual ~Directive();
  Directive(Block* currentContext, const std::string& fullDirectiveLine);
  Directive(const Directive& other);
  Directive& operator=(const Directive& other);

  void validate();
  bool validateContext() const;
  bool validateArgsSize() const;

  virtual bool validateSpecific() = 0;
  virtual void apply(Server& server);
  virtual void apply(Location& location);

  bool getIsValid() const { return isValid; };
  const Block* getCurrentContext() const;
  const std::string& getFullDirectiveLine() const;
  const std::string& getName() const;
  const std::vector<std::string>& getArguments() const;
  int getMinArgs() const;
  int getMaxArgs() const;
  const std::vector<std::string>& getContexts() const;
  bool good() const;

  void setIsValid(bool valid);
  void setFullDirectiveLine(const std::string& line);
  void setName(const std::string& name);
  void setCurrentContext(Block* context);
  void setMinArgs(int min);
  void setMaxArgs(int max);

  void addArgument(const std::string& argument);
  void addContext(const std::string& context);

  private:
  Directive();

  bool isValid;
  Block* currentContext;
  std::string fullDirectiveLine;
  std::string name;
  std::vector<std::string> arguments;
  int minArgs;
  int maxArgs;
  std::vector<std::string> contexts;

  void tokenizeName();
};

class AllowMethodsDirective : public Directive
{
  public:
  ~AllowMethodsDirective();
  AllowMethodsDirective(Block* currentContext, const std::string& fullDirectiveLine);
  AllowMethodsDirective(const AllowMethodsDirective& other);
  AllowMethodsDirective& operator=(const AllowMethodsDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Location& location);

  private:
  AllowMethodsDirective();

  virtual bool isValidMethod(const std::string& method) const;
  virtual void processMethod(const std::string& method);

  bool getAllowed;
  bool postAllowed;
  bool deleteAllowed;
};

class AutoIndexDirective : public Directive
{
  public:
  ~AutoIndexDirective();
  AutoIndexDirective(Block* currentContext, const std::string& fullDirectiveLine);
  AutoIndexDirective(const AutoIndexDirective& other);
  AutoIndexDirective& operator=(const AutoIndexDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Location& location);

  void setAutoIndex(bool autoIndex);

  bool getAutoIndex() const;

  private:
  AutoIndexDirective();

  bool autoIndex;
};

class ClientBodyTempPathDirective : public Directive
{
  public:
  ~ClientBodyTempPathDirective();
  ClientBodyTempPathDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ClientBodyTempPathDirective(const ClientBodyTempPathDirective& other);
  ClientBodyTempPathDirective& operator=(const ClientBodyTempPathDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);
  virtual void apply(Location& location);

  void setTempPath(const std::string& path);

  const std::string& getTempPath() const;

  private:
  ClientBodyTempPathDirective();

  std::string tempPath;
};

class ClientMaxBodySizeDirective : public Directive
{
  public:
  ~ClientMaxBodySizeDirective();
  ClientMaxBodySizeDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ClientMaxBodySizeDirective(const ClientMaxBodySizeDirective& other);
  ClientMaxBodySizeDirective& operator=(const ClientMaxBodySizeDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);
  virtual void apply(Location& location);

  void setMaxBodySize(size_t maxBodySize);

  size_t getMaxBodySize() const;

  private:
  ClientMaxBodySizeDirective();

  int maxBodySize;
};

class ClientTimeOutDirective : public Directive
{
  public:
  ~ClientTimeOutDirective();
  ClientTimeOutDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ClientTimeOutDirective(const ClientTimeOutDirective& other);
  ClientTimeOutDirective& operator=(const ClientTimeOutDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);
  virtual void apply(Location& location);

  void setTimeOut(size_t time);

  size_t getTimeOut() const;

  private:
  ClientTimeOutDirective();

  int time;
};

class DefaultServerDirective : public Directive
{
  public:
  ~DefaultServerDirective();
  DefaultServerDirective(Block* currentContext, const std::string& fullDirectiveLine);
  DefaultServerDirective(const DefaultServerDirective& other);
  DefaultServerDirective& operator=(const DefaultServerDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);

  private:
  DefaultServerDirective();
};

class ErrorPageDirective : public Directive
{
  public:
  ~ErrorPageDirective();
  ErrorPageDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ErrorPageDirective(const ErrorPageDirective& other);
  ErrorPageDirective& operator=(const ErrorPageDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);

  void setErrorPage(const std::string& errorPage);

  std::string getErrorPage() const;

  private:
  ErrorPageDirective();

  int errorCode;
  std::string errorPath;
};

class IndexDirective : public Directive
{
  public:
  ~IndexDirective();
  IndexDirective(Block* currentContext, const std::string& fullDirectiveLine);
  IndexDirective(const IndexDirective& other);
  IndexDirective& operator=(const IndexDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Location& location);

  void setIndexPath(const std::string& index);

  const std::string& getIndexPath() const;

  private:
  IndexDirective();

  std::string indexPath;
};

class ListenDirective : public Directive
{
  public:
  ~ListenDirective();
  ListenDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ListenDirective(const ListenDirective& other);
  ListenDirective& operator=(const ListenDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);

  void setPort(int port);

  int getPort() const;

  private:
  ListenDirective();

  int port;
};

class ReturnDirective : public Directive
{
  public:
  ~ReturnDirective();
  ReturnDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ReturnDirective(const ReturnDirective& other);
  ReturnDirective& operator=(const ReturnDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Location& location);

  void setReturnCode(int returnCode);
  void setReturnPath(const std::string& path);

  int getReturnCode() const;
  const std::string& getReturnPath() const;

  private:
  ReturnDirective();

  int returnCode;
  std::string returnPath;
};

class RootDirective : public Directive
{
  public:
  ~RootDirective();
  RootDirective(Block* currentContext, const std::string& fullDirectiveLine);
  RootDirective(const RootDirective& other);
  RootDirective& operator=(const RootDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Location& location);

  void setRootPath(const std::string& rootPath);
  const std::string& getRootPath() const;

  private:
  RootDirective();

  std::string rootPath;
};

class ServerNameDirective : public Directive
{
  public:
  ~ServerNameDirective();
  ServerNameDirective(Block* currentContext, const std::string& fullDirectiveLine);
  ServerNameDirective(const ServerNameDirective& other);
  ServerNameDirective& operator=(const ServerNameDirective& other);

  virtual bool validateSpecific();
  virtual void apply(Server& server);

  void setServerName(const std::string& serverName);

  const std::string& getServername() const;

  private:
  ServerNameDirective();

  std::string serverName;
};