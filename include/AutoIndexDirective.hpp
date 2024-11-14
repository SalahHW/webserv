/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexDirective.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:38:52 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/14 14:58:20 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"

class AutoIndexDirective : public Directive {
public:
	~AutoIndexDirective();
	AutoIndexDirective(Block* currentContext, const std::string& fullDirectiveLine);
	AutoIndexDirective(const AutoIndexDirective& other);
	AutoIndexDirective& operator=(const AutoIndexDirective& other);

	virtual bool validateSpecific();
	virtual void displayInfo() const;
	virtual void apply(Location& location);

	void setAutoIndex(bool autoIndex);

	bool getAutoIndex() const;
	
private:
	AutoIndexDirective();

	bool autoIndex;
};