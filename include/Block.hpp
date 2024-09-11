/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 03:23:20 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/11 18:02:40 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class Block {
public:
    ~Block();
    Block(const string& name);
    Block(const string& name, Block* parent);

    void print(int indent = 0) const;
    string const& getName() const;
    void addSubBlock(Block* block);
    void addDirective(const string& directive);

private:
    Block();
    Block(Block const& other);
    Block& operator=(Block const& other);

    string name;
    vector<Block*> subBlocks;
    vector<string> directives;
};