/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:22:24 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/13 18:11:26 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HeaderBuilder.hpp"
#include "HttpRequest.hpp"
#include <fstream>
#include <sstream>
#include <string>

class ResponseHandler {
public:
    ResponseHandler(const RequestParsed& requestParsed);
    ~ResponseHandler();

    void handleResponse();
    std::string getResponse() const;

private:
    const RequestParsed& requestParsed;
    HeaderBuilder headerBuilder;
    std::string body;
    std::string fullResponse;

    void buildResponse();
    std::string determineFilePath() const;
    void prepareSuccessResponse(const std::string& filePath);
    void prepareErrorResponse(int statusCode);
    void assembleFullResponse(int statusCode);

    std::string getReasonPhrase(int code) const;
    std::string getFilePathFromUri(const std::string& uri) const;
    bool fileExists(const std::string& filePath) const;
    std::string getContentType(const std::string& filePath) const;
    std::string readFile(const std::string& filePath) const;
};
