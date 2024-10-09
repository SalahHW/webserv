/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveFactory.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:49:08 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/09 11:59:44 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

#include "Directive.hpp"
#include "ClientMaxBodySizeDirective.hpp"
#include "ErrorPageDirective.hpp"
#include "ListenDirective.hpp"
#include "RootDirective.hpp"
#include "ServerNameDirective.hpp"
#include "IndexDirective.hpp"
#include "AutoIndexDirective.hpp"
#include "DenyDirective.hpp"
#include "ReturnDirective.hpp"

class DirectiveFactory {
public:
    ~DirectiveFactory();
    DirectiveFactory();

    Directive* create(const std::string& name, const std::string& context) const;

    template <typename T>
    void registerDirective(const std::string& name)
    {
        factoryMap[name] = &createInstance<T>;
    }

private:
    DirectiveFactory(const DirectiveFactory& other);
    DirectiveFactory& operator=(const DirectiveFactory& other);

    template <typename T>
    static Directive* createInstance(const std::string& context)
    {
        return new T(context);
    }

    std::map<std::string, Directive* (*)(const std::string&)> factoryMap;
};