/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:03:41 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/06 16:03:42 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientIn.hpp"
#include "EpollManagement.hpp"
#include "ServerIn.hpp"

int main()
{
    ServerIn Server;
    EpollManagement Epoll(Server.getListenSockFd());
}