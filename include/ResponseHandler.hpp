/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:22:24 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/07 14:08:43 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include "HeaderBuilder.hpp"
#include "HttpRequest.hpp"
#include <string>

class ResponseHandler {
public:
    ResponseHandler(Client& client, const RequestParsed& requestParsed);
    ~ResponseHandler();

    void handleResponse();

private:
    Client& client;
    const RequestParsed& requestParsed;
    HeaderBuilder headerBuilder;
    std::string body;
    int statusCode;
    std::string reasonPhrase;

    void buildResponse();
    void handleSuccess();
    void handleError();
    void loadErrorPage();
    void sendResponse();
    void closeConnectionIfNeeded();
    std::string getReasonPhrase(int code) const;
};