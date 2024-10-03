/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:10 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/06 16:02:18 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTEXCEPTION_HPP
#define CLIENTEXCEPTION_HPP

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

class ClientException : public std::runtime_error {
public:
    explicit ClientException(const std::string& excp)
        : std::runtime_error(excp)
    {
        // exit(EXIT_FAILURE);
    }
};

#endif