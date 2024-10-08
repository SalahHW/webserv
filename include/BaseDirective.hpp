/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseDirective.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbouheni <sbouheni@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:49:51 by sbouheni          #+#    #+#             */
/*   Updated: 2024/10/08 17:54:49 by sbouheni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class BaseDirective {
public:
    virtual ~BaseDirective();
    BaseDirective(const std::string& currentContext);
    BaseDirective(const BaseDirective& other);
    BaseDirective& operator=(const BaseDirective& other);

    bool validate();
    bool validateContext() const;
    bool validateArgsSize() const;
    
    virtual bool validateSpecific() = 0;

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

private:
    BaseDirective();

    void tokenizeName();
    
    std::string fullDirectiveLine;
    std::string name;
    std::string currentContext;
    std::vector<std::string> arguments;
    int minArgs;
    int maxArgs;
    std::vector<std::string> contexts;
};
