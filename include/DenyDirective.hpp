#pragma once

#include "Directive.hpp"

class DenyDirective : public Directive {
public:
    ~DenyDirective();
    DenyDirective(Block* currentContext, const std::string& fullDirectiveLine);
    DenyDirective(const DenyDirective& other);
    DenyDirective& operator=(const DenyDirective& other);

    virtual bool validateSpecific();
    virtual void displayInfo() const;

private:
    DenyDirective();

    bool getAccepted;
    bool postAccepted;
    bool deleteAccepted;
};