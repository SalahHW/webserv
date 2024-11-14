/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyDirective.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:48:43 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/14 14:39:01 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"

class DenyDirective : public Directive {
public:
    ~DenyDirective();
    DenyDirective(Block* currentContext, const std::string& fullDirectiveLine);
    DenyDirective(const DenyDirective& other);
    DenyDirective& operator=(const DenyDirective& other);

    virtual bool validateSpecific();
    virtual void displayInfo() const;

private:
    DenyDirective();
};