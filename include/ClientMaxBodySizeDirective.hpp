#pragma once

#include "Directive.hpp"
#include "utils.hpp"

class ClientMaxBodySizeDirective : public Directive {
public:
    ~ClientMaxBodySizeDirective();
    ClientMaxBodySizeDirective(const std::string& currentContext, const std::string& fullDirectiveLine);
    ClientMaxBodySizeDirective(const ClientMaxBodySizeDirective& other);
    ClientMaxBodySizeDirective& operator=(const ClientMaxBodySizeDirective& other);

    virtual bool validateSpecific();
    virtual void displayInfo() const;
    virtual void apply(Server& server);
    virtual void apply(Location& location);

    void setMaxBodySize(size_t maxBodySize);

    size_t getMaxBodySize() const;

private:
    ClientMaxBodySizeDirective();

    size_t maxBodySize;
};