/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParsingException.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:03:10 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/06 16:03:12 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPPARSINGEXCEPTION_HPP
#define HTTPPARSINGEXCEPTION_HPP

#include <cstdlib>
#include <stdexcept>

class HttpParsingException : public std::runtime_error {
public:
    explicit HttpParsingException(const std::string& excp)
        : std::runtime_error(excp)
    {
    }
};

#endif
