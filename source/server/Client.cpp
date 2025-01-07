#include "Client.hpp"

Client::~Client()
{
}

Client::Client(int fd)
    : fd(fd)
    , destinationFd(-1)
{
}

void Client::setDestinationFd(int destinationFd)
{
    this->destinationFd = destinationFd;
}

std::string& Client::getBuffer()
{
    return buffer;
}

int Client::getDestinationFd() const
{
    return this->destinationFd;
}

void Client::closeConnection()
{
    close(fd);
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