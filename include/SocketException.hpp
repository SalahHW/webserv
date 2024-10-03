/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:03:49 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/06 16:03:50 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <cstdlib>
#include <stdexcept>

class SocketException : public std::runtime_error {
public:
    explicit SocketException(const std::string& excp)
        : std::runtime_error(excp)
    {
        // exit(EXIT_FAILURE);
    }
};

#endif