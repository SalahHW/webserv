/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageDirective.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:21:19 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 08:46:19 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"
#include "utils.hpp"
#include <map>

class ErrorPageDirective : public Directive {
public:
    ~ErrorPageDirective();
    ErrorPageDirective(const std::string& currentContext);
    ErrorPageDirective(const ErrorPageDirective& other);
    ErrorPageDirective& operator=(const ErrorPageDirective& other);

    virtual bool validateSpecific();
    virtual void apply(Server& server);

    void setErrorPage(const std::string& errorPage);

    std::string getErrorPage() const;

private:
    ErrorPageDirective();

    std::map<int, std::string> errorPages;
};