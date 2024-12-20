/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:35 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/10 08:57:52 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>

class Location {
public:
    ~Location();
    Location();
    Location(const Location& src);
    Location& operator=(const Location& src);

    // Setters
    void setPath(const std::string& path);
    void setRootDirectory(const std::string& rootDirectory);
    void setIndexFile(const std::string& indexFile);
    void setRedirectionPath(const std::string& redirectionPath);
    void setAutoIndex(bool autoIndex);
    void setGetAccepted(bool getAccepted);
    void setPostAccepted(bool postAccepted);
    void setDeleteAccepted(bool deleteAccepted);
    void setClientMaxBodySize(int size);
    void setReturnCode(int returnCode);
    void addPath(const std::string& path);

    // Getters
    const std::string& getPath() const;
    const std::string& getRootDirectory() const;
    const std::string& getIndexFile() const;
    const std::string& getRedirectionPath() const;
    bool getAutoIndex() const;
    bool getGetAccepted() const;
    bool getPostAccepted() const;
    bool getDeleteAccepted() const;
    int getClientMaxBodySize() const;
    int getReturnCode() const;
    const std::vector<std::string>& getPaths() const;

    void displayLocationInfo() const;

private:
    std::string path;
    std::string rootDirectory;
    std::string indexFile;
    std::string redirectionPath;
    bool autoIndex;
    bool getAccepted;
    bool postAccepted;
    bool deleteAccepted;
    int clientMaxBodySize;
    int returnCode;
    std::vector<std::string> paths;
};

#endif
