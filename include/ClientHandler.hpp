#pragma once

#include "Client.hpp"
#include <map>

class ClientHandler {
    public:
        ClientHandler();
        ~ClientHandler();
        ClientHandler(const ClientHandler &other);
        ClientHandler &operator=(const ClientHandler &other);

        void addClient(int id);
        void removeClient(int id);
        void handleClient(int id);
    private:
        std::map<int, Client> clients;
};