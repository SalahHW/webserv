/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:20:11 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 10:30:06 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::~Location() { }

Location::Location() { }

Location::Location(const Location& src)
{
	*this = src;
}

Location& Location::operator=(const Location& src)
{
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
	}
	return *this;
}

void Location::setPath(const std::string& path)
{
	this->path = path;
}

void Location::setRootDirectory(const std::string& rootDirectory)
{
	this->rootDirectory = rootDirectory;
}

void Location::setIndexFile(const std::string& indexFile)
{
	this->indexFile = indexFile;
}

void Location::setRedirectionPath(const std::string& redirectionPath)
{
	this->redirectionPath = redirectionPath;
}

void Location::setAutoIndex(bool autoIndex)
{
	this->autoIndex = autoIndex;
}

void Location::setGetAccepted(bool getAccepted)
{
	this->getAccepted = getAccepted;
}	

void Location::setPostAccepted(bool postAccepted)
{
	this->postAccepted = postAccepted;
}	

void Location::setDeleteAccepted(bool deleteAccepted)
{
	this->deleteAccepted = deleteAccepted;
}

void Location::setClientMaxBodySize(int size)
{
	this->clientMaxBodySize = size;
}

void Location::setReturnCode(int returnCode)
{
	this->returnCode = returnCode;
}

const std::string& Location::getPath() const
{
	return this->path;
}

const std::string& Location::getRootDirectory() const
{
	return this->rootDirectory;
}

const std::string& Location::getIndexFile() const
{
	return this->indexFile;
}

const std::string& Location::getRedirectionPath() const
{
	return this->redirectionPath;
}

bool Location::getAutoIndex() const
{
	return this->autoIndex;
}

bool Location::getGetAccepted() const
{
	return this->getAccepted;
}

bool Location::getPostAccepted() const
{
	return this->postAccepted;
}

bool Location::getDeleteAccepted() const
{
	return this->deleteAccepted;
}

int Location::getClientMaxBodySize() const
{
	return this->clientMaxBodySize;
}

int Location::getReturnCode() const
{
	return this->returnCode;
}

void Location::displayLocationInfo() const
{
    std::cout << "Location " << path << " info" << std::endl;
    std::cout << "- Path: " << path << std::endl;
    std::cout << "- Root Directory: " << rootDirectory << std::endl;
    std::cout << "- Index File: " << indexFile << std::endl;
    std::cout << "- Redirection Path: " << redirectionPath << std::endl;
    std::cout << "- Auto Index: " << (autoIndex ? "Enabled" : "Disabled") << std::endl;
    std::cout << "- GET Accepted: " << (getAccepted ? "Yes" : "No") << std::endl;
    std::cout << "- POST Accepted: " << (postAccepted ? "Yes" : "No") << std::endl;
    std::cout << "- DELETE Accepted: " << (deleteAccepted ? "Yes" : "No") << std::endl;
    std::cout << "- Client Max Body Size: " << clientMaxBodySize << " bytes" << std::endl;
}