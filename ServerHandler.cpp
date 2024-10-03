/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:49 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/03 13:24:21 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerHandler.hpp"

ServerHandler::ServerHandler()
{
}

ServerHandler::~ServerHandler()
{
}

void ServerHandler::addServer(int serverFd, const Server& src)
{
    this->serverList.insert(std::make_pair(serverFd, src));
}