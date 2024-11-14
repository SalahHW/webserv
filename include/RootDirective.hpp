/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootDirective.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:28:59 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/14 15:01:05 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"

class RootDirective : public Directive {
public:
	~RootDirective();
	RootDirective(Block* currentContext, const std::string& fullDirectiveLine);
	RootDirective(const RootDirective& other);
	RootDirective& operator=(const RootDirective& other);

	virtual bool validateSpecific();
	virtual void displayInfo() const;
	virtual void apply(Location& location);

	void setRootPath(const std::string& rootPath);
	const std::string& getRootPath() const;
	
private:
	RootDirective();

	//TODO: Check if this is the correct type for rootPath
	std::string rootPath;
};