/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndexDirective.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:35:02 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 16:51:49 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"

class IndexDirective : public Directive {
public:
    ~IndexDirective();
    IndexDirective(const std::string& currentContext, const std::string& fullDirectiveLine);
    IndexDirective(const IndexDirective& other);
    IndexDirective& operator=(const IndexDirective& other);

    virtual bool validateSpecific();
    virtual void displayInfo() const;
    virtual void apply(Location& location);

    void setIndexPath(const std::string& index);

    const std::string& getIndexPath() const;

private:
    IndexDirective();

    std::string indexPath;
};
