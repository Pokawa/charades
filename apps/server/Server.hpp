//
// Created by hubert on 26.12.2020.
//

//#ifndef CHARADES_SERVER_HPP
//#define CHARADES_SERVER_HPP
#pragma once

#include <string>
#include <unordered_set>

class Server {
private:
    std::string port;
    int serverSocket{};
    std::unordered_set<int> clientsSockets;

public:
    using ServerExeption = std::logic_error;

    explicit Server(std::string port);
    void openServer();
    bool acceptClient();

private:

};


//#endif //CHARADES_SERVER_HPP
