/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 08:56:54 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/16 08:45:23 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationBlock.hpp"

LocationBlock::~LocationBlock() { }

LocationBlock::LocationBlock(const std::string& fullBlockLine, Block* contextBlock)
    : Block(fullBlockLine, contextBlock)
{
    addValidContext("server");
	addValidContext("location");
	validate();
	tokenizeName();
}

LocationBlock::LocationBlock(const LocationBlock& other)
    : Block(other)
{
}

LocationBlock& LocationBlock::operator=(const LocationBlock& other)
{
	if (this != &other) {
		Block::operator=(other);
	}
	return *this;
}

void LocationBlock::validate()
{
	isValid = validateContext();
	// TODO: Path validation
}

void LocationBlock::tokenizeName()
{
    std::stringstream ss(getFullBlockLine());
    std::string token;

    ss >> token;
	ss >> token;
	path = token;

	if (ss >> token) {
		std::cerr << "Error: Unexpected token \"" << token << "\" in location block. in \"" << contextBlock->getName() << "\" block" << std::endl;
		isValid = false;
	}
}