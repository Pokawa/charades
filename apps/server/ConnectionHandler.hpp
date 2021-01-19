//
// Created by hubert on 26.12.2020.
//

#ifndef CHARADES_CONNECTIONHANDLER_HPP
#define CHARADES_CONNECTIONHANDLER_HPP

#include <string>
#include <vector>
#include <poll.h>
#include "Socket.hpp"

class ConnectionHandler {
private:
    std::string port;
    int serverSocket;
    std::vector<chs::Socket> clientsSockets;
    std::vector<pollfd> pollSockets;

    static std::unique_ptr<ConnectionHandler> instance;

public:
    void operator=(const ConnectionHandler &) = delete;
    ConnectionHandler(ConnectionHandler &other) = delete;
    static ConnectionHandler& getInstance(const std::string& port = "2000");

    explicit ConnectionHandler(std::string port);
    void openServer();
    bool acceptClient();
    const chs::Socket& getLastClient();
    const chs::Socket& getSocket(const int &socket);
    std::vector<pollfd> getPollSockets();
    void closeClient(chs::Socket& client);
    void setForWrite(const chs::Socket& client);
    void unsetForWrite(const chs::Socket & client);

private:
    static void setSocketToNonBlock(int socket);
    void addToPoll(int socket, short events);
    void removeFromPoll(int socket);
    pollfd & getPollFd(const chs::Socket & client);
};


#endif //CHARADES_CONNECTIONHANDLER_HPP
