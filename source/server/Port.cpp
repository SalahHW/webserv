#include "Port.hpp"

Port::~Port() { }

Port::Port()
    : isValid(true)
    , port(-1)
    , listenFd(-1)
{
    memset(&addr, 0, sizeof(addr));
	setupSocket();
	bindSocket();
	makeSocketNonBlocking();
}

Port::Port(const Port& src) { *this = src; }

Port& Port::operator=(const Port& src)
{
    if (this != &src) {
        this->isValid = src.isValid;
        this->port = src.port;
        this->listenFd = src.listenFd;
    }
    return *this;
}

void Port::setupSocket()
{
    this->listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listenFd == -1) {
        std::cerr << "Error: Socket creation failed" << std::endl;
        this->isValid = false;
    }
    int optval = 1;
    if (setsockopt(this->listenFd, SOL_SOCKET, SO_REUSEADDR,
            &optval, sizeof(optval))
        == -1) {
        std::cerr << "Error: setsockopt" << std::endl;
        this->isValid = false;
    }
}

void Port::bindSocket()
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->port);
    addr.sin_addr.s_addr = INADDR_ANY;
	int bindResult = bind(this->listenFd, (struct sockaddr*)&addr, sizeof(addr));
    if ( bindResult == -1 ) {
        std::cerr << "Error: Port binding failed" << std::endl;
        this->isValid = false;
    }
}

void Port::makeSocketNonBlocking() {
    int flags = fcntl(this->listenFd, F_GETFL, 0);
    if(flags == -1) {
        std::cerr << "Error: fcntl" << std::endl;
        this->isValid = false;
    }
    flags |= O_NONBLOCK;
    if(fcntl(this->listenFd, F_SETFL, flags) == -1) {
        std::cerr << "Error: fcntl flags" << std::endl;
        this->isValid = false;
    }
}

void Port::startListening()
{
   if (listen(this->listenFd, SOMAXCONN) == -1) {
        std::cerr << "Error: Failed to listen on port " << this->port << std::endl;
        this->isValid = false;
    }
}

void Port::setPort(int port) { this->port = port; }

void Port::setListenFd(int fd) { this->listenFd = fd; }

int Port::getPort() const { return this->port; }

int Port::getListenFd() const { return this->listenFd; }

bool Port::good() const { return this->isValid; }