//
// Created by hubert on 26.12.2020.
//
#include <spdlog/spdlog.h>
#include "ConnectionHandler.hpp"


int main(int argc, char** argv){
    if(argc != 2) {
        spdlog::critical("Invalid parameters. Usage: {} <port>", argv[0]);
        return 1;
    }

    ConnectionHandler server{argv[1]};
    server.acceptClient();
}
