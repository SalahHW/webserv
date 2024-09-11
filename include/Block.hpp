/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 03:23:20 by sbouheni          #+#    #+#             */
/*   Updated: 2024/09/11 03:38:10 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Block {
public:
	~Block();
	Block(const string &name);
private:
	Block();
	Block(Block const &other);
	Block &operator=(Block const &other);
	
    string name;
    map<string, Block*> subBlocks;

    void addSubBlock(Block& block);
    void print(int indent) const;
};