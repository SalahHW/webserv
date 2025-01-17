#include "Client.hpp"

Client::~Client()
{
}

Client::Client(int listenFd, int connectionFd)
    : listenFd(listenFd)
    , connectionFd(connectionFd)
{
}

std::string& Client::getBuffer()
{
    return buffer;
}

int Client::getListenFd() const
{
    return this->listenFd;
}

int Client::getConnectionFd() const
{
    return this->connectionFd;
}

void Client::closeConnection()
{
    close(connectionFd);
    clearBuffer();
}

void Client::appendToBuffer(const char* data, size_t len)
{
    buffer.append(data, len);
}

void Client::clearBuffer()
{
    buffer.clear();
}