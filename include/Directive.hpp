#pragma once

#include "Server.hpp"
#include "Location.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Block;

class Directive {
public:
    virtual ~Directive();
    Directive(Block* currentContext, const std::string& fullDirectiveLine);
    Directive(const Directive& other);
    Directive& operator=(const Directive& other);

    void validate();
    bool validateContext() const;
    bool validateArgsSize() const;

    virtual bool validateSpecific() = 0;
    virtual void displayInfo() const = 0;
    virtual void apply(Server& server);
    virtual void apply(Location& location);

    void setFullDirectiveLine(const std::string& line);
    void setName(const std::string& name);
    void setCurrentContext(Block* context);
    void addArgument(const std::string& argument);
    void addContext(const std::string& context);
    void setMinArgs(int min);
    void setMaxArgs(int max);

    std::string getFullDirectiveLine() const;
    std::string getName() const;
    Block* getCurrentContext() const;
    std::vector<std::string> getArguments() const;
    std::vector<std::string> getContexts() const;
    int getMinArgs() const;
    int getMaxArgs() const;
    bool good() const;

protected:

    void tokenizeName();

    bool isValid;
    Block* currentContext;
    std::string fullDirectiveLine;
    std::string name;
    std::vector<std::string> arguments;
    int minArgs;
    int maxArgs;
    std::vector<std::string> contexts;

private:
    Directive();
};
