/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:46 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/04 10:30:12 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

// #include "Location.hpp"

class Location; // TODO: remove after Location.hpp is created

class Server {
public:
    ~Server();
    Server();

    void setListenFd(int fd);
    void setPort(int port);
    void setCLientMaxBodySize(int size);
    void setName(const std::string& name);
    void setErrorPages(std::vector<std::string> error);

    void addLocation(const Location& location);

    int getListenFd() const;
    int getPort() const;
    int getCLientMaxBodySize() const;
    const std::string& getName() const;
    const std::vector<std::string>& getErrorPages() const;

private:
    Server(const Server& src);
    Server& operator=(const Server& src);
    
    int listenFd;
    int port;
    std::string name;
    int clientMaxBodySize;
    std::vector<std::string> errorPages; // TODO: Use map instead to store error codes and their corresponding pages
    std::vector<Location> locations;
};

#endif