/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 08:07:01 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/16 10:12:03 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

ServerBlock::~ServerBlock() { }

ServerBlock::ServerBlock(const std::string& fullBlockLine, Block* contextBlock)
	: Block(fullBlockLine, contextBlock)
{
	addValidContext("main");
	addValidContext("http");
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

void ServerBlock::validate()
{
	isValid = validateContext(); // && check arguments
}