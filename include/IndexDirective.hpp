/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndexDirective.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:35:02 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 18:36:15 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseDirective.hpp"

class IndexDirective : public BaseDirective {
public:
    ~IndexDirective();
    IndexDirective(const std::string& currentContext);
    IndexDirective(const IndexDirective& other);
    IndexDirective& operator=(const IndexDirective& other);

    virtual bool validateSpecific();

    void setIndexPath(const std::string& index);

    const std::string& getIndexPath() const;

private:
    IndexDirective();

    std::string indexPath;
};
