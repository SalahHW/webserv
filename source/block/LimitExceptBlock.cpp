/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptBlock.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:09:17 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/06 20:08:42 by sbouheni         ###   ########.fr       */
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

    isGetConcerned = false;
    isPostConcerned = false;
    isDeleteConcerned = false;
    validate();
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
    return extractArguments();
}

bool LimitExceptBlock::extractArguments()
{
    for (size_t i = 0; i < arguments.size(); i++) {
        if (arguments[i] == "GET") {
            isGetConcerned = true;
        } else if (arguments[i] == "POST") {
            isPostConcerned = true;
        } else if (arguments[i] == "DELETE") {
            isDeleteConcerned = true;
        } else {
            std::cerr << "Error: " << "limit_expect block has an invalid argument \"" << arguments[i] << "\"" << std::endl;
            return false;
        }
    }
    return true;
}