/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 04:17:01 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/17 05:49:19 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"

using namespace std;

Block::~Block()
{
    for (size_t i = 0; i < subBlocks.size(); ++i) {
        delete subBlocks[i];
    }
}

Block::Block(const string& name)
    : name(name)
{
    tokenizeName();
    cout << tokenizedName[0] << " block created" << endl;
}

Block::Block(const string& name, Block* parent)
    : name(name)
{
    tokenizeName();
    cout << tokenizedName[0] << " block created and added to " << parent->name << endl;
    parent->addSubBlock(this);
}

void Block::print(int indent) const
{
    for (int i = 0; i < indent; ++i)
        std::cout << "  ";
    std::cout << "Block: " << name << std::endl;

    for (size_t i = 0; i < directives.size(); ++i) {
        for (int j = 0; j < indent + 1; ++j)
            std::cout << "  ";
        std::cout << "Directive: " << directives[i].getName() << std::endl;
    }

    for (size_t i = 0; i < subBlocks.size(); ++i) {
        subBlocks[i]->print(indent + 1);
    }
}

string const& Block::getName() const
{
    return name;
}

void Block::addSubBlock(Block* block)
{
    subBlocks.push_back(block);
}

void Block::addDirective(const Directive& directive)
{
    directives.push_back(directive);
}

void Block::tokenizeName()
{
    vector<string> tokens;
    stringstream ss(name);
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }
    tokenizedName = tokens;
}