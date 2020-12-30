//
// Created by hubert on 26.12.2020.
//

#ifndef CHARADES_CONNECTIONHANDLER_HPP
#define CHARADES_CONNECTIONHANDLER_HPP

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
    const chs::WebSocket& getWebSocket(const int &socket);
    std::vector<pollfd> getPollSockets();
    void closeClient(const chs::WebSocket& client);
    void setForWrite(const chs::WebSocket& client);
    void unsetForWrite(const chs::WebSocket & client);

private:
    static void setSocketToNonBlock(int socket);
    void addToPoll(int socket, short events);
    void removeFromPoll(int socket);
    pollfd & getPollFd(const chs::WebSocket & client);
};


#endif //CHARADES_CONNECTIONHANDLER_HPP
