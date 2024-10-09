/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNameDirective.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:05:45 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/09 11:59:44 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Directive.hpp"

class ServerNameDirective : public Directive {
public:
    ~ServerNameDirective();
    ServerNameDirective(const std::string& currentContext);
    ServerNameDirective(const ServerNameDirective& other);
    ServerNameDirective& operator=(const ServerNameDirective& other);

    virtual bool validateSpecific();

    void setServerName(const std::string& serverName);

    const std::string& getServername() const;

private:
    ServerNameDirective();

    std::string serverName;
};