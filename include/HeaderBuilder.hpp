/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderBuilder.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:43:54 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/13 16:40:45 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Constants.hpp"
#include <map>
#include <string>

class HeaderBuilder {
public:
    HeaderBuilder();
    ~HeaderBuilder();

    void setStatusCode(int code);
    void setReasonPhrase(const std::string& phrase);

    void addHeader(const std::string& key, const std::string& value);
    std::string buildStatusLine() const;
    std::string buildHeaders() const;

private:
    int statusCode;
    std::string reasonPhrase;
    std::map<std::string, std::string> headers;
};