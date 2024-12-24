#include "Client.hpp"

Client::~Client()
{
}

Client::Client(int fd)
    : fd(fd)
{
}

std::string& Client::getBuffer() {
	return buffer;
}

void Client::closeConnection() {
	close(fd);
	clearBuffer();
}

void Client::appendToBuffer(const char* data, size_t len) {
	buffer.append(data, len);
}

void Client::clearBuffer()
{
	buffer.clear();
}