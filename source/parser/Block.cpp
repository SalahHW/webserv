/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 04:17:01 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 17:44:58 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Block.hpp"

using namespace std;

Block::~Block()
{
    for (size_t i = 0; i < subBlocks.size(); ++i) {
        delete subBlocks[i];
    }
    for (size_t i = 0; i < directives.size(); ++i) {
        delete directives[i];
    }
}

Block::Block()
{
    isRoot = true;
    cout << "Root block created" << endl;
}

Block::Block(const string& name, Block* parent)
    : name(name)
    , isRoot(false)
{
    tokenizeName();
    this->name = tokenizedName[0];
    cout << this->name << " block created and added to " << parent->name << endl;
    parent->addSubBlock(this);
}

void Block::printBlock(int indent) const
{
    for (int i = 0; i < indent; ++i)
        std::cout << "  ";
    std::cout << "Block: " << name << std::endl;

    for (size_t i = 0; i < directives.size(); ++i) {
        for (int j = 0; j < indent + 1; ++j)
            std::cout << "  ";
        std::cout << "Directive: " << directives[i]->getFullDirectiveLine() << std::endl;
    }

    for (size_t i = 0; i < subBlocks.size(); ++i) {
        subBlocks[i]->printBlock(indent + 1);
    }
}

string const& Block::getName() const
{
    return (this->name);
}

vector<string> const& Block::getTokenizedName() const
{
    return (this->tokenizedName);
}

vector<Block*> const& Block::getSubBlocks() const
{
    return (this->subBlocks);
}

vector<BaseDirective*> const& Block::getDirectives() const
{
    return (this->directives);
}

bool Block::isRootBlock() const
{
    return (this->isRoot);
}

void Block::addSubBlock(Block* block)
{
    subBlocks.push_back(block);
}

void Block::addDirective(BaseDirective* directive)
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