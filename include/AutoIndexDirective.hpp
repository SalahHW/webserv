/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexDirective.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:38:52 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/09 11:59:44 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"

class AutoIndexDirective : public Directive {
public:
	~AutoIndexDirective();
	AutoIndexDirective(const std::string& currentContext);
	AutoIndexDirective(const AutoIndexDirective& other);
	AutoIndexDirective& operator=(const AutoIndexDirective& other);

	virtual bool validateSpecific();

	void setAutoIndex(bool autoIndex);

	bool getAutoIndex() const;
	
private:
	AutoIndexDirective();

	bool autoIndex;
};