#ifndef SERVER_HPP
#define SERVER_HPP

class Server {
private:
    int listen_fd;
    int port;
    std::string name;
    int clientMaxBodySize;
    std::vestor<std::string> ErrorPages;
    std::vector<Locationobj> routes;
}