#include "Port.hpp"

Port::~Port()
{
}

Port::Port()
    : isValid(true)
    , port(-1)
    , listenFd(-1)
{
}

// Port::Port(const Port& src) { *this = src; }

// Port& Port::operator=(const Port& src)
// {
//     if (this != &src) {
//         this->isValid = src.isValid;
//         this->port = src.port;
//         this->listenFd = src.listenFd;
//         this->virtualHosts = src.virtualHosts;
//         this->addr = src.addr;
//     }
//     return *this;
// }

void Port::initialize() {
    memset(&addr, 0, sizeof(addr));
    setupSocket();
    bindSocket();
    makeSocketNonBlocking();
    // startListening();
}

void Port::setupSocket()
{
    this->listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->listenFd == -1) {
        std::cerr << "Error: Socket creation failed" << std::endl;
        this->isValid = false;
    }
    int optval = 1;
    if (setsockopt(this->listenFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
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
    if (bindResult == -1) {
        std::cerr << "Error: Port binding failed on port " << this->port << std::endl;
        this->isValid = false;
    }
}

void Port::makeSocketNonBlocking()
{
    int flags = fcntl(this->listenFd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Error: fcntl" << std::endl;
        this->isValid = false;
    }
    flags |= O_NONBLOCK;
    if (fcntl(this->listenFd, F_SETFL, flags) == -1) {
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

void Port::addVirtualHost(const Server& server)
{
    const std::string& serverName = server.getName();

    if (virtualHosts.find(serverName) != virtualHosts.end()) {
        std::cerr << "Warning: VirtualHost \"" << serverName
                  << "\" already exists on port " << port << std::endl;
    }

    VirtualHost newHost = createVirtualHost(server);
    virtualHosts[serverName] = newHost;
}

VirtualHost Port::createVirtualHost(const Server& server)
{
    VirtualHost newHost;

    newHost.setName(server.getName());
    newHost.setClientMaxBodySize(server.getClientMaxBodySize());
    newHost.setErrorPages(server.getErrorPages());
    newHost.setLocations(server.getLocations());

    return newHost;
}

bool Port::good() const { return this->isValid; }

void Port::displayHosts() const
{
    std::cout << "Printing virtual Hosts on port " << port << ":" << std::endl;
    std::map<std::string, VirtualHost>::const_iterator itHost;

    for (itHost = virtualHosts.begin(); itHost != virtualHosts.end(); ++itHost) {
        const VirtualHost host = itHost->second;

        std::cout << "File Descriptor: " << this->getListenFd() << std::endl;
        std::cout << "Host Name: " << host.getName() << std::endl;
        std::cout << "Client Max Body Size: " << host.getClientMaxBodySize() << std::endl;
        std::cout << "Number of locations: " << host.getLocations().size() << std::endl
                  << std::endl;
    }
}

int Port::getPort() const { return this->port; }

int Port::getListenFd() const { return this->listenFd; }

const std::map<std::string, VirtualHost>& Port::getVirtualHosts() const { return this->virtualHosts; }

void Port::setPort(int port) { this->port = port; }

void Port::setListenFd(int fd) { this->listenFd = fd; }