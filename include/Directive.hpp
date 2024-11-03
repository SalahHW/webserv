/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directive.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:49:51 by sbouheni          #+#    #+#             */
/*   Updated: 2024/11/03 08:01:42 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Location.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Directive {
public:
    virtual ~Directive();
    Directive(const std::string& currentContext, const std::string& fullDirectiveLine);
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
    void setCurrentContext(const std::string& context);
    void addArgument(const std::string& argument);
    void addContext(const std::string& context);
    void setMinArgs(int min);
    void setMaxArgs(int max);

    std::string getFullDirectiveLine() const;
    std::string getName() const;
    std::string getCurrentContext() const;
    std::vector<std::string> getArguments() const;
    std::vector<std::string> getContexts() const;
    int getMinArgs() const;
    int getMaxArgs() const;
    bool getIsValid() const;

protected:
    Directive();

    void tokenizeName();

    bool isValid;
    std::string currentContext;
    std::string fullDirectiveLine;
    std::string name;
    std::vector<std::string> arguments;
    int minArgs;
    int maxArgs;
    std::vector<std::string> contexts;
};
