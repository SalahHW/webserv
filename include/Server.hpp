/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:46 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/03 13:32:05 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

class Server {
private:
    int listenFd;
    int port;
    std::string name;
    int clientMaxBodySize;
    std::vector<std::string> ErrorPages;
    // std::vector<Locationrouteobj> routes;
    Server& operator=(const Server& src);
    Server(const Server& src);

public:
    Server();
    ~Server();
    void setListenFd(int fd);
    void setPort(int port);
    void setCLientMaxBodySize(int size);
    void setName(const std::string& name);
    void setErrorPages(std::vector<std::string> error);
    int getListenFd() const;
    int getPort() const;
    int getCLientMaxBodySize() const;
    const std::string& getName() const;
    const std::vector<std::string>& getErrorPages() const;
};

#endif