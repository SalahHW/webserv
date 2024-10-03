/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollException.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:38 by joakoeni          #+#    #+#             */
/*   Updated: 2024/09/06 16:02:40 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLEXCEPTION_HPP
#define EPOLLEXCEPTION_HPP

#include <cstdlib>
#include <stdexcept>

class EpollException : public std::runtime_error {
public:
    explicit EpollException(const std::string& excp)
        : std::runtime_error(excp)
    {
        // exit(EXIT_FAILURE);
    }
};

#endif