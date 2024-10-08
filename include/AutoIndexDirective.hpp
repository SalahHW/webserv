/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndexDirective.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:38:52 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:41:15 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseDirective.hpp"

class AutoIndexDirective : public BaseDirective {
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