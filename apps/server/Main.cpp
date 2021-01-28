//
// Created by hubert on 26.12.2020.
//
#include <spdlog/spdlog.h>
#include <fcntl.h>
#include "ConnectionHandler.hpp"
#include "IOHandler.hpp"
#include "PlayersHandler.hpp"
#include "RoomsHandler.hpp"
#include "LogicHandler.hpp"

int main(int argc, char** argv){
    if(argc != 2) {
        spdlog::critical("Invalid parameters. Usage: {} <port>", argv[0]);
        return 1;
    }

    auto & connectionHandler = ConnectionHandler::getInstance(argv[1]);
    connectionHandler.openServer();
    auto & ioHandler = IOHandler::getInstance();
    auto & playersHandler = PlayersHandler::getInstance();

    while(true) {
        auto pollSockets = ConnectionHandler::getInstance().getPollSockets();
        auto ret = poll(pollSockets.data(), pollSockets.size(), -1);

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

        for (auto* player : playersHandler.getPlayers()) {
            LogicHandler logicHandler{player};
            while (ioHandler.isMessageToGet(player->getSocket())){
                auto message = ioHandler.getMessage(player->getSocket());
                logicHandler.handleMessage(std::move(message));
            }
        }

        for (int i = 1; i < pollSockets.size(); ++i) {
            auto client = connectionHandler.getSocket(pollSockets[i].fd);
            while (ioHandler.isMessageToGet(client)) {
                auto message = ioHandler.getMessage(client);

                switch (chs::getMessageType(message)) {
                    case chs::MessageType::LOG_IN_REQUEST: {
                        auto [name] = chs::deconstructMessage<std::string>(message);

                        if (playersHandler.isNameAvailable(name)){
                            playersHandler.addPlayer(name, client);
                            spdlog::info("Logged in player: {} on {}", name, client.getPort());
                            LogicHandler::sendSimpleRespond(client, chs::MessageType::OK_RESPOND);
                        } else {
                            spdlog::info("Refused player: {} on {}; Name in use", name, client.getPort());
                            LogicHandler::sendSimpleRespond(client, chs::MessageType::ERROR_RESPOND);
                        }
                    }
                        break;
                    case chs::MessageType::LOG_OUT_REQUEST: {
                        connectionHandler.closeClient(client);
                        spdlog::info("Log out from: {}", client.getPort());
                    }
                }


            }
        }

    }
    return 0;
}
