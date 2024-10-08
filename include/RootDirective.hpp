/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RootDirective.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:28:59 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:32:33 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseDirective.hpp"

class RootDirective : public BaseDirective {
public:
	~RootDirective();
	RootDirective(const std::string& currentContext);
	RootDirective(const RootDirective& other);
	RootDirective& operator=(const RootDirective& other);

	virtual bool validateSpecific();

	void setRootPath(const std::string& rootPath);
	const std::string& getRootPath() const;
	
private:
	RootDirective();

	//TODO: Check if this is the correct type for rootPath
	std::string rootPath;
};