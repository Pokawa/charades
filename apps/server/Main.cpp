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
    IOHandler ioHandler;

    while(true) {
        auto pollSockets = connectionHandler.getPollSockets();
        poll(pollSockets.data(), pollSockets.size(), -1);

        if (pollSockets[0].revents & POLLIN) {
            if (connectionHandler.acceptClient());
                ioHandler.addClient(connectionHandler.getLastClient());
        }

        for (int i = 1; i < pollSockets.size(); ++i) {
            if (pollSockets[i].revents & POLLIN) {
                auto client = connectionHandler.getWebSocket(pollSockets[i].fd);
                ioHandler.receiveFrom(client);

                if (ioHandler.isMessageToGet(client)) {
                    auto message = ioHandler.getMessage(client);
                    spdlog::info("Received message from {} : \"{}\"", client.getAddress(), message);
                }
            }
        }
    }
    return 0;
}
