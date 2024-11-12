/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderBuilder.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:43:54 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/07 13:44:07 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>

class HeaderBuilder {
public:
    HeaderBuilder();
    ~HeaderBuilder();

    void addHeader(const std::string& key, const std::string& value);
    std::string buildHeaders() const;

private:
    std::map<std::string, std::string> headers;
};