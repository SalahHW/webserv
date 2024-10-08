/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMaxBodySizeDirective.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:16:05 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:17:54 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseDirective.hpp"
#include "utils.hpp"

class ClientMaxBodySizeDirective : public BaseDirective {
public:
    ~ClientMaxBodySizeDirective();
    ClientMaxBodySizeDirective(const std::string& currentContext);
    ClientMaxBodySizeDirective(const ClientMaxBodySizeDirective& other);
    ClientMaxBodySizeDirective& operator=(const ClientMaxBodySizeDirective& other);

    virtual bool validateSpecific();

    void setMaxBodySize(int maxBodySize);

    int getMaxBodySize() const;

private:
    ClientMaxBodySizeDirective();

    int maxBodySize;
};