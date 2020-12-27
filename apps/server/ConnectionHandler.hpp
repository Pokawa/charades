//
// Created by hubert on 26.12.2020.
//

//#ifndef CHARADES_SERVER_HPP
//#define CHARADES_SERVER_HPP
#pragma once

#include <string>
#include <vector>
#include "WebSocket.h"

class ConnectionHandler {
private:
    std::string port;
    int serverSocket;
    std::vector<chs::WebSocket> clientsSockets;

public:
    explicit ConnectionHandler(std::string port);
    void openServer();
    bool acceptClient();

private:

};


//#endif //CHARADES_SERVER_HPP
