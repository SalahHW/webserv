/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:35 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/07 10:09:07 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>

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

    void addLocation(const Location& location);

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
};

#endif