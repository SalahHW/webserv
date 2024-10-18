/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainBlock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 09:14:19 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/16 09:20:28 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MainBlock.hpp"

MainBlock::~MainBlock() { }

MainBlock::MainBlock()
    : Block("main", NULL)
{
    minArgs = 0;
    maxArgs = 0;
    validate();
}

MainBlock::MainBlock(const MainBlock& other)
	: Block(other)
{
}

MainBlock& MainBlock::operator=(const MainBlock& other)
{
    if (this != &other) {
        Block::operator=(other);
    }
    return *this;
}

bool MainBlock::validateSpecific()
{
    return true;
}
