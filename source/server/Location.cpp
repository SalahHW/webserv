#include "Location.hpp"

Location::~Location() {}

Location::Location()
    : getAccepted(true), postAccepted(true), deleteAccepted(true) {
  this->hasPath = false;
  this->hasRootDirectory = false;
  this->hasIndexFile = false;
  this->hasRedirectionPath = false;
  this->hasAutoIndex = false;
  this->hasGetAccepted = false;
  this->hasPostAccepted = false;
  this->hasDeleteAccepted = false;
  this->hasClientMaxBodySize = false;
  this->hasReturnCode = false;
  this->hasPaths = false;
}

Location::Location(const Location &src) { *this = src; }

Location &Location::operator=(const Location &src) {
  if (this != &src) {
    this->path = src.path;
    this->rootDirectory = src.rootDirectory;
    this->indexFile = src.indexFile;
    this->redirectionPath = src.redirectionPath;
    this->autoIndex = src.autoIndex;
    this->getAccepted = src.getAccepted;
    this->postAccepted = src.postAccepted;
    this->deleteAccepted = src.deleteAccepted;
    this->clientMaxBodySize = src.clientMaxBodySize;
    this->returnCode = src.returnCode;
    this->paths = src.paths;
  }
  return *this;
}

void Location::setPath(const std::string &path) {
  this->path = path;
  this->hasPath = true;
}

void Location::setRootDirectory(const std::string &rootDirectory) {
  this->rootDirectory = rootDirectory;
  this->hasRootDirectory = true;
}

void Location::setIndexFile(const std::string &indexFile) {
  this->indexFile = indexFile;
  this->hasIndexFile = true;
}

void Location::setRedirectionPath(const std::string &redirectionPath) {
  this->redirectionPath = redirectionPath;
  this->hasRedirectionPath = true;
}

void Location::setAutoIndex(bool autoIndex) { this->autoIndex = autoIndex; }

void Location::setGetAccepted(bool getAccepted) {
  this->getAccepted = getAccepted;
  this->hasGetAccepted = true;
}

void Location::setPostAccepted(bool postAccepted) {
  this->postAccepted = postAccepted;
  this->hasPostAccepted = true;
}

void Location::setDeleteAccepted(bool deleteAccepted) {
  this->deleteAccepted = deleteAccepted;
  this->hasDeleteAccepted = true;
}

void Location::setClientMaxBodySize(int size) {
  this->clientMaxBodySize = size;
  this->hasClientMaxBodySize = true;
}

void Location::setReturnCode(int returnCode) {
  this->returnCode = returnCode;
  this->hasReturnCode = true;
}

void Location::addPath(const std::string &path) {
  this->paths.push_back(path);
  this->hasPaths = true;
}

const std::string &Location::getPath() const { return this->path; }

const std::string &Location::getRootDirectory() const {
  return this->rootDirectory;
}

const std::string &Location::getIndexFile() const { return this->indexFile; }

const std::string &Location::getRedirectionPath() const {
  return this->redirectionPath;
}

bool Location::getAutoIndex() const { return this->autoIndex; }

bool Location::getGetAccepted() const { return this->getAccepted; }

bool Location::getPostAccepted() const { return this->postAccepted; }

bool Location::getDeleteAccepted() const { return this->deleteAccepted; }

int Location::getClientMaxBodySize() const { return this->clientMaxBodySize; }

int Location::getReturnCode() const { return this->returnCode; }

const std::vector<std::string> &Location::getPaths() const {
  return this->paths;
}

void Location::displayLocationInfo() const {
  std::cout << "Location \"" << path << "\"" << std::endl;
  if (isPathDefined()) std::cout << "- Path: " << path << std::endl;
  if (isRootDirectoryDefined())
    std::cout << "- Root Directory: " << rootDirectory << std::endl;
  if (isIndexFileDefined())
    std::cout << "- Index File: " << indexFile << std::endl;
  if (isRedirectionPathDefined())
    std::cout << "- Redirection Path: " << redirectionPath << std::endl;
  if (isReturnCodeDefined())
    std::cout << "- Return Code: " << returnCode << std::endl;
  if (isAutoIndexDefined())
    std::cout << "- Auto Index: " << (autoIndex ? "Enabled" : "Disabled")
              << std::endl;
  if (isGetAcceptedDefined())
    std::cout << "- GET Accepted: " << (getAccepted ? "Yes" : "No")
              << std::endl;
  if (isPostAcceptedDefined())
    std::cout << "- POST Accepted: " << (postAccepted ? "Yes" : "No")
              << std::endl;
  if (isDeleteAcceptedDefined())
    std::cout << "- DELETE Accepted: " << (deleteAccepted ? "Yes" : "No")
              << std::endl;
  if (isClientMaxBodySizeDefined())
    std::cout << "- Client Max Body Size: " << clientMaxBodySize << " bytes"
              << std::endl;
}
