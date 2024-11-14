/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectiveFactory.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:49:37 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/14 15:06:05 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DirectiveFactory.hpp"

DirectiveFactory::~DirectiveFactory() { }

DirectiveFactory::DirectiveFactory()
{
    registerDirective<ListenDirective>("listen");
    registerDirective<ServerNameDirective>("server_name");
    registerDirective<ClientMaxBodySizeDirective>("client_max_body_size");
    registerDirective<ErrorPageDirective>("error_page");
    registerDirective<RootDirective>("root");
    registerDirective<IndexDirective>("index");
    registerDirective<AutoIndexDirective>("autoindex");
    registerDirective<ReturnDirective>("return");
}

Directive* DirectiveFactory::create(const std::string& name, Block* context, const std::string& fullDirectiveline) const
{
    std::map<std::string, Directive* (*)(Block*, const std::string&)>::const_iterator it = factoryMap.find(name);
    if (it != factoryMap.end()) {
        return it->second(context, fullDirectiveline);
    }
    return NULL;
}
