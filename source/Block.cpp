/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 03:24:48 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/11 03:30:08 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"

Block::~Block()
{
}

Block::Block(const string& name)
    : name(name)
{
    cout << name << " block created" << endl;
}

void Block::addSubBlock(Block& block)
{
    subBlocks[block.name] = &block;
}

void Block::print(int indent = 0) const
{
    for (int i = 0; i < indent; ++i)
        cout << "  ";
    cout << "Block: " << name << endl;

    for (const auto& subBlockPair : subBlocks) {
        subBlockPair.second->print(indent + 1);
    }
}