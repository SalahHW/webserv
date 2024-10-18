/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MainBlock.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 09:14:42 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/12 09:33:57 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Block.hpp"

class MainBlock : public Block {
public:
	virtual ~MainBlock();
	MainBlock();
	MainBlock(const MainBlock& src);
	MainBlock& operator=(const MainBlock& src);

	bool validateSpecific();
	
private:
	
};