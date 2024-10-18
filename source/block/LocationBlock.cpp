/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 08:56:54 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/18 12:02:04 by sbouheni         ###   ########.fr       */
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
	isValid = validateContext() && validateArgsSize() && validateSpecific();
}

bool LocationBlock::validateSpecific()
{
	std::set<std::string> uniquePaths(paths.begin(), paths.end());
	if (uniquePaths.size() != paths.size()) {
		std::cerr << "Error: Duplicate paths found in location block." << std::endl;
		return false;
	}
	return true;
}

void LocationBlock::tokenizeName()
{
    std::stringstream ss(getFullBlockLine());
    std::string token;

    ss >> token;

	while (ss >> token) {
			paths.push_back(token);
	}
}

const std::vector<std::string>& LocationBlock::getPaths() const
{
	return paths;
}

void LocationBlock::apply(Location& location) const
{
    for (size_t i = 0; i < paths.size(); ++i) {
        location.addPath(paths[i]);
    }
}
