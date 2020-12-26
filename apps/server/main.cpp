//
// Created by hubert on 26.12.2020.
//
#include <spdlog/spdlog.h>
#include "Server.hpp"


int main(int argc, char** argv){
    if(argc != 2) {
        spdlog::critical("Invalid parameters. Usage: {} <port>", argv[0]);
        return 1;
    }

    Server server{argv[1]};
    try {
        server.openServer();
    } catch (Server::ServerExeption& exception) {
        spdlog::critical(exception.what());
        exit(1);
    }
    server.acceptClient();
}
