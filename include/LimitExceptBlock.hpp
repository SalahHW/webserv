/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitExceptBlock.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:04:25 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/06 13:59:48 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Block.hpp"
#include <map>

class LimitExceptBlock : public Block {
public:
    virtual ~LimitExceptBlock();
    LimitExceptBlock(const std::string& fullBlockLine, Block* contextBlock);
    LimitExceptBlock(const LimitExceptBlock& other);
    LimitExceptBlock& operator=(const LimitExceptBlock& other);

    virtual bool validateSpecific();

protected:
    bool isGetConcerned;
    bool isPostConcerned;
    bool isDeleteConcerned;

private:
	bool extractArguments();
};