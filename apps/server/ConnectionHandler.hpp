//
// Created by hubert on 26.12.2020.
//

//#ifndef CHARADES_SERVER_HPP
//#define CHARADES_SERVER_HPP
#pragma once

#include <string>
#include <vector>
#include <poll.h>
#include "WebSocket.hpp"

class ConnectionHandler {
private:
    std::string port;
    int serverSocket;
    std::vector<chs::WebSocket> clientsSockets;
    std::vector<pollfd> pollSockets;

public:
    explicit ConnectionHandler(std::string port);
    void openServer();
    bool acceptClient();
    const chs::WebSocket& getLastClient();
    const std::vector<pollfd> & getPollSockets();

private:
    static void setSocketToNonBlock(int socket);
    void addToPoll(int socket, short events);
    void removeFromPoll(int socket);
};


//#endif //CHARADES_SERVER_HPP
