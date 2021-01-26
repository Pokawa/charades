//
// Created by hubert on 26.12.2020.
//
#include <spdlog/spdlog.h>
#include <fcntl.h>
#include "ConnectionHandler.hpp"
#include "IOHandler.hpp"
#include "PlayersHandler.hpp"
#include "RoomsHandler.hpp"

int main(int argc, char** argv){
    if(argc != 2) {
        spdlog::critical("Invalid parameters. Usage: {} <port>", argv[0]);
        return 1;
    }

    auto & connectionHandler = ConnectionHandler::getInstance(argv[1]);
    connectionHandler.openServer();
    auto & ioHandler = IOHandler::getInstance();
    auto & playersHandler = PlayersHandler::getInstance();
    auto & roomsHandler = RoomsHandler::getInstance();

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

        for (int i = 1; i < pollSockets.size(); ++i) {
            auto client = connectionHandler.getSocket(pollSockets[i].fd);

            while (ioHandler.isMessageToGet(client)) {
                auto message = ioHandler.getMessage(client);
                auto type = chs::getMessageType(message);

                if (type == chs::MessageType::LOG_IN_REQUEST) {
                    auto [name] = chs::deconstructMessage<std::string>(message);

                    if (playersHandler.isNameAvailable(name)){
                        playersHandler.addPlayer(name, client);
                        spdlog::info("Logged in player: {} from: {}", name, client.getAddress());
                        auto respondMessage = chs::constructMessage(chs::MessageType::OK_RESPOND);
                        ioHandler.putMessage(client, respondMessage);
                    } else {
                        spdlog::info("Refused player: {} from: {}; Name in use", name, client.getAddress());
                        auto respondMessage = chs::constructMessage(chs::MessageType::ERROR_RESPOND);
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

                    auto inGameInfoMessage = player.getRoom().getInGameInfo();
                    ioHandler.putMessage(player.getRoom().getPlayers(), inGameInfoMessage);
                }

                if (type == chs::MessageType::JOIN_ROOM_REQUEST) {
                    auto& player = playersHandler.getPlayer(client);
                    auto [roomNumber] = chs::deconstructMessage<int>(message);

                    if (roomsHandler.roomExists(roomNumber)) {
                        auto respondMessage = chs::constructMessage(chs::MessageType::OK_RESPOND);
                        ioHandler.putMessage(client, respondMessage);
                        roomsHandler.joinRoom(roomNumber, &player);
                        spdlog::info("{} joined room {}", player.name, roomNumber);

                        auto inGameInfoMessage = player.getRoom().getInGameInfo();
                        ioHandler.putMessage(player.getRoom().getPlayers(), inGameInfoMessage);
                    } else {
                        auto respondMessage = chs::constructMessage(chs::MessageType::ERROR_RESPOND);
                        ioHandler.putMessage(client, respondMessage);
                        spdlog::warn("{} failed to join room {}: room not found", player.name, roomNumber);
                    }

                }

                if (type == chs::MessageType::QUIT_ROOM_REQUEST) {
                    auto& player = playersHandler.getPlayer(client);
                    auto roomNumber = player.getRoom().getRoomNumber();
                    spdlog::info("Player {} quit room {}", player.name, roomNumber);
                    roomsHandler.quitRoom(&player);
                    if (roomsHandler.roomExists(roomNumber)) {
                        auto& room = roomsHandler.getRoomByNumber(roomNumber);
                        auto inGameInfoMessage = room.getInGameInfo();
                        ioHandler.putMessage(room.getPlayers(), inGameInfoMessage);
                    }
                }

                if (type == chs::MessageType::LOG_OUT_REQUEST) {
                    if (playersHandler.clientIsLoggedIn(client)) {
                        auto& player = playersHandler.getPlayer(client);

                        if (player.isInRoom()) {
                            auto roomNumber = player.getRoom().getRoomNumber();
                            spdlog::info("Player {} quit room {}", player.name, roomNumber);
                            roomsHandler.quitRoom(&player);
                            if (roomsHandler.roomExists(roomNumber)) {

                                auto& room = roomsHandler.getRoomByNumber(roomNumber);
                                auto inGameInfoMessage = room.getInGameInfo();
                                ioHandler.putMessage(room.getPlayers(), inGameInfoMessage);
                            }
                        }

                        playersHandler.removePlayer(client);
                        spdlog::info("Removed player: {}", player.name);
                    }

                    connectionHandler.closeClient(client);
                    spdlog::info("Log out from: {}", client.getAddress());
                    continue;
                }

                if (type == chs::MessageType::CHAT_MESSAGE) {
                    auto& player = playersHandler.getPlayer(client);
                    auto players = player.getRoom().getPlayers();

                    auto [chat] = chs::deconstructMessage<std::string>(message);
                    auto newChatMessage = chs::constructMessage(chs::MessageType::CHAT_MESSAGE,
                                                                fmt::format("{} : {}"), player.name, chat);

                    ioHandler.putMessage(players, newChatMessage);
                }

                if (type == chs::MessageType::DRAW_LINE) {
                    auto& player = playersHandler.getPlayer(client);
                    auto players = player.getRoom().getPlayersButOne(&player);
                    auto renewedMessage = chs::addSizeToMessage(message);
                    ioHandler.putMessage(players, renewedMessage);
                }

                if (type == chs::MessageType::CLEAR_DRAWING) {
                    auto& player = playersHandler.getPlayer(client);
                    auto players = player.getRoom().getPlayersButOne(&player);
                    auto renewedMessage = chs::addSizeToMessage(message);
                    ioHandler.putMessage(players, renewedMessage);
                }

                if (type == chs::MessageType::ENTER_DRAWING_QUEUE_REQUEST) {
                    auto& player = playersHandler.getPlayer(client);
                    auto position = player.getRoom().getInDrawingQueue(&player);
                    auto respondMessage = chs::constructMessage(chs::MessageType::SERVER_MESSAGE,
                                                                fmt::format("You are {} in queue", position));
                    ioHandler.putMessage(player.getSocket(), respondMessage);
                }

                if (type == chs::MessageType::QUIT_DRAWING_QUEUE_REQUEST) {
                    auto& player = playersHandler.getPlayer(client);
                    player.getRoom().quitDrawingQueue(&player);
                }

                if (type == chs::MessageType::START_GAME_REQUEST) {
                    auto& player = playersHandler.getPlayer(client);
                    auto& room = player.getRoom();

                    if (room.getNumberOfPlayers() >= 2) {
                        room.startGame();
                        auto inGameInfoMessage = room.getInGameInfo();
                        ioHandler.putMessage(room.getPlayers(), inGameInfoMessage);
                        ioHandler.putMessage(room.getDrawer()->getSocket(), room.getCharadesWordMessage());
                        //TODO setup timer callbacks
                    }
                }
            }
        }

    }
    return 0;
}
