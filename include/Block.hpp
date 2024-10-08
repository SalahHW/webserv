/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 03:23:20 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 17:41:55 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseDirective.hpp"
#include "Directive.hpp"
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <sstream>

class Block {
public:
    ~Block();
    Block();
    Block(const std::string& name, Block* parent);

    void printBlock(int indent = 0) const;
    std::string const& getName() const;
    std::vector<std::string> const& getTokenizedName() const;
    std::vector<Block*> const& getSubBlocks() const;
    std::vector<BaseDirective *> const& getDirectives() const;
    bool isRootBlock() const;
    void addSubBlock(Block* block);
    void addDirective(BaseDirective* directive);

private:
    Block(Block const& other);
    Block& operator=(Block const& other);
    
    void tokenizeName();

    std::string name;
    bool isRoot;
    std::vector<std::string> tokenizedName;
    std::vector<Block*> subBlocks;
    std::vector<BaseDirective*> directives;
    std::vector<std::string> validBlockNames;
};