/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BlockFactory.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 07:55:24 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/05 14:24:38 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BlockFactory.hpp"

BlockFactory::~BlockFactory() { }

BlockFactory::BlockFactory()
{
    registerBlock<ServerBlock>("server");
    registerBlock<LocationBlock>("location");
    registerBlock<LimitExceptBlock>("limit_except");
}

Block* BlockFactory::create(const std::string& name, Block* contextBlock, const std::string& fullBlockLine) const
{
    std::map<std::string, Block* (*)(Block*, const std::string&)>::const_iterator it = factoryMap.find(name);
    if (it != factoryMap.end()) {
        return it->second(contextBlock, fullBlockLine);
    }
    return NULL;
}

MainBlock* BlockFactory::createMainBlock() const
{
    return new MainBlock();
}
