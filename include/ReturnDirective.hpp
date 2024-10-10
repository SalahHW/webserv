/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReturnDirective.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:53:38 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 08:59:14 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"

class ReturnDirective : public Directive {
public:
    ~ReturnDirective();
    ReturnDirective(const std::string& currentContext);
    ReturnDirective(const ReturnDirective& other);
    ReturnDirective& operator=(const ReturnDirective& other);

    virtual bool validateSpecific();
    virtual void apply(Location& location);

    void setReturnCode(int returnCode);
    void setReturnPath(const std::string& path);

    int getReturnCode() const;
    const std::string& getReturnPath() const;

private:
    ReturnDirective();

    int returnCode;
    std::string returnPath;
};