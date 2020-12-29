//
// Created by hubert on 26.12.2020.
//
#include <spdlog/spdlog.h>
#include "ConnectionHandler.hpp"
#include "IOHandler.hpp"


int main(int argc, char** argv){
    if(argc != 2) {
        spdlog::critical("Invalid parameters. Usage: {} <port>", argv[0]);
        return 1;
    }

    ConnectionHandler connectionHandler{argv[1]};

    while(true) {
        auto pollSockets = connectionHandler.getPollSockets();
        poll(pollSockets.data(), pollSockets.size(), -1);

        if (pollSockets[0].revents & POLLIN) {
            if (connectionHandler.acceptClient());

        }


    }
}
