#include "ServerManager.hpp"

ServerManager::~ServerManager()
{
}

ServerManager::ServerManager()
{
}

ServerManager::ServerManager(std::map<int, Port*> ports)
    : isValid(true)
    , ports(ports)
    , isRunning(false)
{
    if (ports.empty()) {
        std::cerr << "Error: No ports provided for server manager." << std::endl;
        return;
    }
    if (!initializePorts()) {
        isValid = false;
        return;
    }
    makePortsListening();
    addPortsToEventReporter();
}

ServerManager::ServerManager(const ServerManager& other)
{
    *this = other;
}

ServerManager& ServerManager::operator=(const ServerManager& other)
{
    if (this != &other) {
        this->ports = other.ports;
        this->isRunning = other.isRunning;
    }
    return *this;
}

bool ServerManager::good() const {
    return this->isValid;
}

void ServerManager::start()
{
    eventReporter.run(&ServerManager::handleEvent, this);
}

bool ServerManager::initializePorts()
{
    std::map<int, Port*>::iterator itPort;

    for (itPort = ports.begin(); itPort != ports.end(); ++itPort) {
        itPort->second->initialize();
        if (!itPort->second->good()) {
            std::cerr << "Ports initialization failed." << std::endl;
            return false;
        }
    }
    return true;
}

void ServerManager::makePortsListening()
{
    std::map<int, Port*>::iterator itPort;

    for (itPort = ports.begin(); itPort != ports.end(); ++itPort) {
        itPort->second->startListening();
    }
}

void ServerManager::addPortsToEventReporter()
{
    std::map<int, Port*>::iterator itPort;

    for (itPort = ports.begin(); itPort != ports.end(); ++itPort) {
        int portFd = itPort->second->getListenFd();
        if (!eventReporter.addFD(portFd)) {
            return;
        }
    }
}

void ServerManager::handleEvent(int fd, uint32_t events)
{
    if (events & EPOLLIN) {
        std::cout << "Socket " << fd << ": Data ready to read" << std::endl;
    }
    if (events & EPOLLOUT) {
        std::cout << "Socket " << fd << ": Ready to write" << std::endl;
    }
    if (events & EPOLLERR) {
        std::cerr << "Socket " << fd << ": Error occurred" << std::endl;
    }
}