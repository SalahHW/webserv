/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 08:33:24 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/16 08:45:07 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Block.hpp"
#include <sstream>
#include <vector>

class LocationBlock : public Block {
public:
    virtual ~LocationBlock();
    LocationBlock(const std::string& fullBlockLine, Block* contextBlock);
    LocationBlock(const LocationBlock& src);
    LocationBlock& operator=(const LocationBlock& src);

    void validate();
    const std::vector<std::string>& getPaths() const;

private:
	void tokenizeName();

	std::vector<std::string> paths;
}
