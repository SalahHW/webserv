/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderBuilder.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:43:54 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/15 17:22:46 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

class HeaderBuilder {
public:
    HeaderBuilder();
    ~HeaderBuilder();

    void setStatusCode(int code);
    void setReasonPhrase(const std::string& phrase);
    void setContentType(const std::string& contentType);

    void addHeader(const std::string& key, const std::string& value);
    std::string buildStatusLine() const;
    std::string buildHeaders() const;

private:
    int statusCode;
    std::string reasonPhrase;
    std::map<std::string, std::string> headers;

    static const char* HTTP_VERSION;
    static const char* LINE_TERMINATOR;
    static const char* HEADER_SEPARATOR;
    static const int DEFAULT_STATUS_CODE;
    static const char* DEFAULT_REASON_PHRASE;
};