/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 03:23:20 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/17 17:08:49 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

class Block {
public:
    virtual ~Block();
    Block(const std::string& fullBlockLine, Block* contextBlock);
    Block(const Block& other);
    Block& operator=(const Block& other);

    virtual void validate() = 0;
    bool validateContext() const;

    std::string getName() const;
    Block* getContextBlock() const;
    std::string getFullBlockLine() const;
    std::vector<Block*> getSubBlocks() const;
    std::vector<Directive*> getDirectives() const;
    bool good() const;

    void addSubBlock(Block* block);
    void addDirective(Directive* directive);
    void addValidContext(const std::string& context);

    void printBlock(int indent = 0) const;

protected:
    std::stringstream ss;
    bool isValid;
    std::string fullLine;
    Block* contextBlock;
    std::string name;
    std::vector<Block*> subBlocks;
    std::vector<Directive*> directives;
    std::vector<std::string> validContexts;

    void extractName();

private:
    Block();
};