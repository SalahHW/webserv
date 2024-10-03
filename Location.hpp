/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:20:35 by joakoeni          #+#    #+#             */
/*   Updated: 2024/10/03 13:20:35 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>

class Location {
private:
    std::string path;
    std::string rootDirectory;
    std::string indexFile;
    std::string redirectionPath;
    bool autoIndex;
    bool getAccepted;
    bool postAccepted;
    bool deleteAccepted;
    int clientMaxBodySize;
};

#endif