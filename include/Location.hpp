#pragma once

#include <iostream>
#include <string>
#include <vector>

class Location
{
  public:
  ~Location();
  Location();
  Location(const Location& src);
  Location& operator=(const Location& src);

  // Setters
  void setPath(const std::string& pathValue);
  void setRootDirectory(const std::string& rootDirectoryValue);
  void setIndexFile(const std::string& indexFileValue);
  void setRedirectionPath(const std::string& redirectionPathValue);
  void setClientBodyTempPath(const std::string& clientBodyTempPath);
  void setAutoIndex(bool autoIndexValue);
  void setGetAccepted(bool getAcceptedValue);
  void setPostAccepted(bool postAcceptedValue);
  void setDeleteAccepted(bool deleteAcceptedValue);
  void setClientMaxBodySize(int size);
  void setClientTimeOut(int time);
  void setReturnCode(int returnCodeValue);
  void addPath(const std::string& pathValue);

  // Getters
  const std::string& getPath() const;
  const std::string& getRootDirectory() const;
  const std::string& getIndexFile() const;
  const std::string& getRedirectionPath() const;
  const std::string& getClientBodyTempPath() const;
  bool getAutoIndex() const;
  bool getGetAccepted() const;
  bool getPostAccepted() const;
  bool getDeleteAccepted() const;
  int getClientMaxBodySize() const;
  int getClientTimeOut() const;
  int getReturnCode() const;
  const std::vector<std::string>& getPaths() const;

  bool isPathDefined() const { return hasPath; }
  bool isRootDirectoryDefined() const { return hasRootDirectory; }
  bool isIndexFileDefined() const { return hasIndexFile; }
  bool isRedirectionPathDefined() const { return hasRedirectionPath; }
  bool isClientBodyTempPathDefined() const { return hasClientBodyTempPath; }
  bool isAutoIndexDefined() const { return hasAutoIndex; }
  bool isGetAcceptedDefined() const { return hasGetAccepted; }
  bool isPostAcceptedDefined() const { return hasPostAccepted; }
  bool isDeleteAcceptedDefined() const { return hasDeleteAccepted; }
  bool isClientMaxBodySizeDefined() const { return hasClientMaxBodySize; }
  bool isClientTimeOutDefined() const { return hasClientTimeOut; }
  bool isReturnCodeDefined() const { return hasReturnCode; }
  bool isPathsDefined() const { return hasPaths; }

  void displayLocationInfo() const;

  private:
  std::string path;
  std::string rootDirectory;
  std::string indexFile;
  std::string redirectionPath;
  std::string clientBodyTempPath;
  bool autoIndex;
  bool getAccepted;
  bool postAccepted;
  bool deleteAccepted;
  int clientMaxBodySize;
  int clientTimeOut;
  int returnCode;
  std::vector<std::string> paths;

  bool hasPath;
  bool hasRootDirectory;
  bool hasIndexFile;
  bool hasRedirectionPath;
  bool hasClientBodyTempPath;
  bool hasAutoIndex;
  bool hasGetAccepted;
  bool hasPostAccepted;
  bool hasDeleteAccepted;
  bool hasClientMaxBodySize;
  bool hasClientTimeOut;
  bool hasReturnCode;
  bool hasPaths;
};