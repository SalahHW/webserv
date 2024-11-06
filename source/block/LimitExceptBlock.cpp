/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptBlock.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:09:17 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/05 14:23:56 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitExceptBlock.hpp"

LimitExceptBlock::~LimitExceptBlock() { }

LimitExceptBlock::LimitExceptBlock(const std::string& fullBlockLine, Block* contextBlock)
	: Block(fullBlockLine, contextBlock)
{
	addValidContext("location");
    minArgs = 1;
	maxArgs = 3;
}

LimitExceptBlock::LimitExceptBlock(const LimitExceptBlock& other)
	: Block(other)
{
}

LimitExceptBlock& LimitExceptBlock::operator=(const LimitExceptBlock& other)
{
	if (this != &other) {
		Block::operator=(other);
	}
	return *this;
}

bool LimitExceptBlock::validateSpecific()
{
	// Placeholder implementation, update as needed
	return true;
}
