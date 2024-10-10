/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:46 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/10 08:49:53 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Server {
public:
    ~Server();
    Server();
    Server(const Server& src);
    Server& operator=(const Server& src);

    void setListenFd(int fd);
    void setPort(int port);
    void setClientMaxBodySize(int size);
    void setName(const std::string& name);
    void setErrorPages(std::map<int, std::string> errorPages);

    void addLocation(const Location& location);

    int getListenFd() const;
    int getPort() const;
    int getClientMaxBodySize() const;
    const std::string& getName() const;
    const std::map<int, std::string>& getErrorPages() const;

    void displayServerInfo() const;

private:
    int listenFd;
    int port;
    std::string name;
    int clientMaxBodySize;
    std::map<int, std::string> errorPages; // TODO: Use map instead to store error codes and their corresponding pages
    std::vector<Location> locations;
};