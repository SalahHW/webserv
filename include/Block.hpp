/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 03:23:20 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/12 01:55:42 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <vector>

class Block {
public:
    ~Block();
    Block(const std::string& name);
    Block(const std::string& name, Block* parent);

    void print(int indent = 0) const;
    std::string const& getName() const;
    void addSubBlock(Block* block);
    void addDirective(const std::string& directive);

private:
    Block();
    Block(Block const& other);
    Block& operator=(Block const& other);

    std::string name;
    std::vector<Block*> subBlocks;
    std::vector<std::string> directives;
};