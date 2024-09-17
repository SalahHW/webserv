/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 03:23:20 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/17 05:47:54 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <sstream>

class Block {
public:
    ~Block();
    Block(const std::string& name);
    Block(const std::string& name, Block* parent);

    void print(int indent = 0) const;
    std::string const& getName() const;
    std::vector<std::string> const& getTokenizedName() const;
    void addSubBlock(Block* block);
    void addDirective(const Directive& directive);

private:
    Block();
    Block(Block const& other);
    Block& operator=(Block const& other);
    
    void tokenizeName();

    std::string name;
    std::vector<std::string> tokenizedName;
    std::vector<Block*> subBlocks;
    std::vector<Directive> directives;
};