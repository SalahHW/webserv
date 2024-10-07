/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:46 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/07 12:10:15 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <iostream>

#include "Location.hpp"

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
    void setErrorPages(std::vector<std::string> error);

    void addLocation(const Location& location);

    int getListenFd() const;
    int getPort() const;
    int getClientMaxBodySize() const;
    const std::string& getName() const;
    const std::vector<std::string>& getErrorPages() const;

    void displayServerInfo() const;

private:
    
    int listenFd;
    int port;
    std::string name;
    int clientMaxBodySize;
    std::vector<std::string> errorPages; // TODO: Use map instead to store error codes and their corresponding pages
    std::vector<Location> locations;
};

#endif