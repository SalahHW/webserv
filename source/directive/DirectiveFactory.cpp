#include "DirectiveFactory.hpp"

DirectiveFactory::~DirectiveFactory() { }

DirectiveFactory::DirectiveFactory()
{
  registerDirective<ListenDirective>("listen");
  registerDirective<ServerNameDirective>("server_name");
  registerDirective<ClientMaxBodySizeDirective>("client_max_body_size");
  registerDirective<ClientBodyTempPathDirective>("client_body_temp_path");
  registerDirective<ClientTimeOutDirective>("client_timeout");
  registerDirective<ErrorPageDirective>("error_page");
  registerDirective<RootDirective>("root");
  registerDirective<IndexDirective>("index");
  registerDirective<AutoIndexDirective>("autoindex");
  registerDirective<ReturnDirective>("return");
  registerDirective<DefaultServerDirective>("default_server");
  registerDirective<AllowMethodsDirective>("allow_methods");
}

Directive* DirectiveFactory::create(
    const std::string& name, Block* context,
    const std::string& fullDirectiveline) const
{
  std::map<std::string,
      Directive* (*)(Block*, const std::string&)>::const_iterator it
      = factoryMap.find(name);
  if (it != factoryMap.end())
  {
    return it->second(context, fullDirectiveline);
  }
  return NULL;
}
