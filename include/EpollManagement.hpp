/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollManagement.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:57 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/22 16:19:36 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_MANAGEMENT_HPP
#define EPOLL_MANAGEMENT_HPP

#include "ClientIn.hpp"
#include "EpollException.hpp"
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 10

class ClientIn;
class HttpParsing;

class EpollManagement {
private:
    int epoll_fd;
    int nb_events;

public:
    EpollManagement(int listen_sock_fd);
    ~EpollManagement();
    EpollManagement(const EpollManagement& src);
    EpollManagement& operator=(const EpollManagement& src);
    void addToEpoll(int listen_sock_fd) const;
    const int& getEpollFd() const;
    const int& getNbEvents() const;
    void EpollInit();
    void startToListen(int listen_sock_fd);
    void handleClientData(int client_fd) const;
};

#endif