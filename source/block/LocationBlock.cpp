/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 08:56:54 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/18 20:51:22 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationBlock.hpp"
#include "utils.hpp"

LocationBlock::~LocationBlock() { }

LocationBlock::LocationBlock(const std::string& fullBlockLine, Block* contextBlock)
    : Block(fullBlockLine, contextBlock)
{
    addValidContext("server");
	addValidContext("location");
	minArgs = 1;
	maxArgs = 2;
	validate();
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

bool LocationBlock::validateSpecific()
{
	std::set<std::string> uniquePaths(arguments.begin(), arguments.end());
	if (uniquePaths.size() != arguments.size()) {
		std::cerr << "Error: Duplicate paths found in location block." << std::endl;
		return false;
	}
	return true;
}

void LocationBlock::apply(Location& location) const
{
    for (size_t i = 0; i < arguments.size(); ++i) {
        location.addPath(arguments[i]);
    }
}
