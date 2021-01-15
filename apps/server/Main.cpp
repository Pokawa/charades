//
// Created by hubert on 26.12.2020.
//
#include <spdlog/spdlog.h>
#include "ConnectionHandler.hpp"
#include "IOHandler.hpp"
#include "PlayersHandler.hpp"

int main(int argc, char** argv){
    if(argc != 2) {
        spdlog::critical("Invalid parameters. Usage: {} <port>", argv[0]);
        return 1;
    }

    ConnectionHandler connectionHandler{argv[1]};
    IOHandler ioHandler{connectionHandler};
    PlayersHandler playersHandler;

    while(true) {
        auto pollSockets = connectionHandler.getPollSockets();
        poll(pollSockets.data(), pollSockets.size(), -1);

        if (pollSockets[0].revents & POLLIN) {
            if (connectionHandler.acceptClient());
                ioHandler.addClient(connectionHandler.getLastClient());
        }

        for (int i = 1; i < pollSockets.size(); ++i) {
            if (pollSockets[i].revents & POLLIN) {
                auto client = connectionHandler.getSocket(pollSockets[i].fd);
                ioHandler.receiveFrom(client);

                if (ioHandler.isMessageToGet(client)) {
                    auto message = ioHandler.getMessage(client);
                    auto type = chs::getMessageType(message);

                    if (type == chs::MessageType::LOG_IN) {
                        auto [name] = chs::deconstructMessage<std::string>(message);
                        if (playersHandler.isNameAvailable(name)){
                            playersHandler.addPlayer(name, client);
                            spdlog::info("Connected player: {} from: {}", name, client.getAddress());
                            auto message = chs::constructMessage("hubcio xddd");
                            ioHandler.putMessage(client, message);
                        }
                    }

                    if (type == chs::MessageType::LOG_OUT) {
                        connectionHandler.closeClient(client);
                        continue;
                    }
                }
            }

            if (pollSockets[i].revents & POLLOUT) {
                auto client = connectionHandler.getSocket(pollSockets[i].fd);
                ioHandler.sendTo(client);
            }
        }
    }
    return 0;
}
