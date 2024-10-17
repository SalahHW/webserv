/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 08:07:22 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/16 08:43:10 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Block.hpp"

class ServerBlock : public Block {
public:
	virtual ~ServerBlock();
	ServerBlock(const std::string& fullBlockLine, Block* contextBlock);
	ServerBlock(const ServerBlock& src);
	ServerBlock& operator=(const ServerBlock& src);

	void validate();
};