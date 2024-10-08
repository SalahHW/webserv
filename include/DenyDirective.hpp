/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DenyDirective.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:48:43 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:49:14 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseDirective.hpp"

class DenyDirective : public BaseDirective {
public:
    ~DenyDirective();
    DenyDirective(const std::string& currentContext);
    DenyDirective(const DenyDirective& other);
    DenyDirective& operator=(const DenyDirective& other);

    virtual bool validateSpecific();

private:
    DenyDirective();
};