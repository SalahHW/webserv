#pragma once

#include "Directive.hpp"
#include "utils.hpp"
#include <map>

class ErrorPageDirective : public Directive {
public:
    ~ErrorPageDirective();
    ErrorPageDirective(const std::string& currentContext, const std::string& fullDirectiveLine);
    ErrorPageDirective(const ErrorPageDirective& other);
    ErrorPageDirective& operator=(const ErrorPageDirective& other);

    virtual bool validateSpecific();
    virtual void displayInfo() const;
    virtual void apply(Server& server);

    void setErrorPage(const std::string& errorPage);

    std::string getErrorPage() const;

private:
    ErrorPageDirective();

    std::map<int, std::string> errorPages;
};