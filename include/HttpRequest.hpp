/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:20:00 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/14 12:20:00 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

// Inclure les constantes globales
#include "Constants.hpp"

enum HttpStatusCode {
    OK = Constants::OK,
    BAD_REQUEST = Constants::BAD_REQUEST,
    PAGE_NOT_FOUND = Constants::PAGE_NOT_FOUND,
    METHOD_NOT_ALLOWED = Constants::METHOD_NOT_ALLOWED,
    LENGTH_REQUIRED = Constants::LENGTH_REQUIRED,
    REQUEST_ENTITY_TOO_LARGE = Constants::REQUEST_ENTITY_TOO_LARGE,
    INTERNAL_SERVER_ERROR = Constants::INTERNAL_SERVER_ERROR,
    NOT_IMPLEMENTED = Constants::NOT_IMPLEMENTED,
    BAD_GATEWAY = Constants::BAD_GATEWAY,
    SERVICE_UNAVAILABLE = Constants::SERVICE_UNAVAILABLE,
    GATEWAY_TIMEOUT = Constants::GATEWAY_TIMEOUT,
    HTTP_VERSION_NOT_SUPPORTED = Constants::HTTP_VERSION_NOT_SUPPORTED
};

struct RequestParsed {
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
    HttpStatusCode status;
};

class HttpRequest {
private:
    std::string request;
    RequestParsed requestParsed;

    // Méthodes de parsing internes
    bool findAndParseRequestLine(std::string::size_type& headersStartPos);
    bool findAndParseHeaders(std::string::size_type headersStartPos, std::string::size_type& bodyStartPos);
    bool parseRequestBody(std::string::size_type bodyStartPos);

public:
    // Constructeurs et destructeur
    HttpRequest(const std::string& request);
    ~HttpRequest();
    HttpRequest(const HttpRequest& src);
    HttpRequest& operator=(const HttpRequest& src);

    // Accesseurs
    const RequestParsed& getHttpRequest() const;

    // Méthodes de parsing
    void parseHttpRequest();
    void showHttpRequest() const;

    // Vérification des méthodes supportées
    bool isMethodSupported(const std::string& method) const;

private:
    // Méthode utilitaire pour supprimer les espaces blancs
    std::string trim(const std::string& str) const;
};