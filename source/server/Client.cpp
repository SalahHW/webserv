#include "Client.hpp"

Client::~Client()
{
    std::cout << "Client fd " << connectionFd << " connection closed" << std::endl;
}

Client::Client(int listenFd, int connectionFd, Port* port)
    : listenFd(listenFd)
    , connectionFd(connectionFd)
    , associatedPort(port)
{
    std::cout << "New client connected on fd: " << connectionFd << " port: " << port->getPort() << std::endl;
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