/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenDirective.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:15:00 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/10 11:10:44 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Directive.hpp"
#include "utils.hpp"

class ListenDirective : public Directive {
public:
    ~ListenDirective();
    ListenDirective(const std::string& currentContext);
    ListenDirective(const ListenDirective& other);
    ListenDirective& operator=(const ListenDirective& other);

    virtual bool validateSpecific();
    virtual void displayInfo() const;
    virtual void apply(Server& server);

    void setPort(int port);

    int getPort() const;

private:
    ListenDirective();

    int port;
};