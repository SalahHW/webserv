/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveFactory.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:49:08 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/14 14:57:30 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

#include "AutoIndexDirective.hpp"
#include "ClientMaxBodySizeDirective.hpp"
#include "DenyDirective.hpp"
#include "Directive.hpp"
#include "ErrorPageDirective.hpp"
#include "IndexDirective.hpp"
#include "ListenDirective.hpp"
#include "ReturnDirective.hpp"
#include "RootDirective.hpp"
#include "ServerNameDirective.hpp"

class DirectiveFactory {
public:
    ~DirectiveFactory();
    DirectiveFactory();

    Directive* create(const std::string& name, Block* context, const std::string& fullDirectiveline) const;

    template <typename T>
    void registerDirective(const std::string& name)
    {
        factoryMap[name] = &createInstance<T>;
    }

private:
    DirectiveFactory(const DirectiveFactory& other);
    DirectiveFactory& operator=(const DirectiveFactory& other);

    template <typename T>
    static Directive* createInstance(Block* context, const std::string& fullDirectiveLine)
    {
        return new T(context, fullDirectiveLine);
    }

    std::map<std::string, Directive* (*)(Block*, const std::string&)> factoryMap;
};