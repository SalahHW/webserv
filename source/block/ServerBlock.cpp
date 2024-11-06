/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 08:07:01 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/05 14:18:49 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

ServerBlock::~ServerBlock() { }

ServerBlock::ServerBlock(const std::string& fullBlockLine, Block* contextBlock)
	: Block(fullBlockLine, contextBlock)
{
	addValidContext("main");
	addValidContext("http");
	minArgs = 0;
	maxArgs = 0;
}

ServerBlock::ServerBlock(const ServerBlock& other)
	: Block(other)
{
}

ServerBlock& ServerBlock::operator=(const ServerBlock& other)
{
	if (this != &other) {
		Block::operator=(other);
	}
	return *this;
}

bool ServerBlock::validateSpecific()
{
	// Placeholder implementation, update as needed
	return true;
}
