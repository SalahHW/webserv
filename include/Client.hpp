/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joakoeni <joakoeni@student.42mulhouse.f>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:02:32 by joakoeni          #+#    #+#             */
/*   Updated: 2024/11/13 15:00:00 by joakoeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientException.hpp"
#include "Constants.hpp"
#include "HttpRequest.hpp"
#include "ResponseHandler.hpp"
#include <map>
#include <string>

using namespace Constants;

class Client {
public:
    Client(int client_fd);
    ~Client();
    Client& operator=(const Client& src);
    Client(const Client& other);

    const int& getClientFd() const;
    bool shouldCloseConnection() const;
    void setConnectionShouldClose(bool shouldClose);

    void appendToRequestBuffer(const std::string& data);
    bool hasDataToWrite() const;
    ssize_t sendData();

private:
    int client_fd;
    RequestParsed request;
    std::string requestBuffer;
    std::string responseBuffer;
    bool connectionShouldClose;
    size_t bytesSent;

    void processRequest();
    void setResponse(const std::string& response);
    void closeClientSocket();
    void copyClientData(const Client& other);
    bool isRequestComplete() const;
    void parseRequest();
    void handleResponse();
    void prepareForSending();
    void checkConnectionPersistence();
    void handleError(const std::string& functionName);
};
