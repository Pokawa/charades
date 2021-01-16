//
// Created by hubert on 26.12.2020.
//
#include <spdlog/spdlog.h>
#include "ConnectionHandler.hpp"
#include "IOHandler.hpp"
#include "PlayersHandler.hpp"
#include "RoomsHandler.hpp"

int main(int argc, char** argv){
    if(argc != 2) {
        spdlog::critical("Invalid parameters. Usage: {} <port>", argv[0]);
        return 1;
    }

    ConnectionHandler connectionHandler{argv[1]};
    IOHandler ioHandler{connectionHandler};
    PlayersHandler playersHandler;
    RoomsHandler roomsHandler;

    while(true) {
        auto pollSockets = connectionHandler.getPollSockets();
        poll(pollSockets.data(), pollSockets.size(), -1);

        if (pollSockets[0].revents & POLLIN) {
            if (connectionHandler.acceptClient())
                ioHandler.addClient(connectionHandler.getLastClient());
        }

        for (int i = 1; i < pollSockets.size(); ++i) {
            auto client = connectionHandler.getSocket(pollSockets[i].fd);
            if (pollSockets[i].revents & POLLIN) {
                ioHandler.receiveFrom(client);
            }

            if (pollSockets[i].revents & POLLOUT) {
                ioHandler.sendTo(client);
            }
        }

        for (int i = 1; i < pollSockets.size(); ++i) {
            auto client = connectionHandler.getSocket(pollSockets[i].fd);

            while (ioHandler.isMessageToGet(client)) {
                auto message = ioHandler.getMessage(client);
                auto type = chs::getMessageType(message);

                if (type == chs::MessageType::LOG_IN_REQUEST) {
                    auto [name] = chs::deconstructMessage<std::string>(message);

                    if (playersHandler.isNameAvailable(name)){
                        playersHandler.addPlayer(name, client);
                        spdlog::info("Connected player: {} from: {}", name, client.getAddress());
                        auto respondMessage = chs::constructMessage(chs::MessageType::OK_RESPOND);
                        ioHandler.putMessage(client, respondMessage);
                    }
                }

                if (type == chs::MessageType::ROOMS_INFO_REQUEST) {
                    auto roomsInfo = roomsHandler.getRoomsInfo();
                    ioHandler.putMessages(client, roomsInfo);
                    spdlog::info("Sent rooms info to: {}", client.getAddress());
                }

                if (type == chs::MessageType::NEW_ROOM_REQUEST) {
                    auto& player = playersHandler.getPlayer(client);
                    roomsHandler.newRoom(&player);
                    spdlog::info("Created new room for: {}[{}]", player.name, client.getAddress());
                    //TODO send in-room data
                }

                if (type == chs::MessageType::LOG_OUT_REQUEST) {
                    connectionHandler.closeClient(client);
                    spdlog::info("Log out from: {}", client.getAddress());
                    continue;
                }
            }
        }

    }
    return 0;
}
