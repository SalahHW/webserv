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
#include "utils.hpp"

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
	std::set<std::string> uniquePaths(paths.begin(), paths.end());
	if (uniquePaths.size() != paths.size()) {
		isValid = false;
		std::cerr << "Error: Duplicate paths found in location block." << std::endl;
	}
	for (size_t i = 0; i < paths.size(); ++i) {
		if (!utils::isValidPathFormat(paths[i])) {
			isValid = false;
			std::cerr << "Error: Invalid path format: " << paths[i] << std::endl;
		}
	}
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
