//
// Created by hubert on 28.01.2021.
//

#include <Message.hpp>
#include "LogicHandler.hpp"


LogicHandler::LogicHandler(Player *player) : player(player),
                                             playerName(player->name),
                                             socket(player->getSocket()),
                                             roomsHandler(RoomsHandler::getInstance()),
                                             playersHandler(PlayersHandler::getInstance()),
                                             ioHandler(IOHandler::getInstance()),
                                             connectionHandler(ConnectionHandler::getInstance()) {
}

void LogicHandler::handleMessage(chs::Message message) {

    switch (chs::getMessageType(message)) {
        case chs::MessageType::LOG_OUT_REQUEST: {
            safelyQuitRoom();
            playersHandler.removePlayer(socket);
            connectionHandler.closeClient(socket);
            spdlog::info("Removed player: {}", playerName);
            spdlog::info("Log out from: {}", playerName);
        }
            break;
        case chs::MessageType::ROOMS_INFO_REQUEST: {
            auto roomsInfo = roomsHandler.getRoomsInfo();
            ioHandler.putMessages(socket, roomsInfo);
            spdlog::info("Sent rooms info to: {}", player->name);
        }
            break;
        case chs::MessageType::NEW_ROOM_REQUEST: {
            roomsHandler.newRoom(player);
            spdlog::info("Created new room for: {}", player->name);
            sendInGameInfo();
        }
            break;
        case chs::MessageType::JOIN_ROOM_REQUEST: {
            auto[roomNumber] = chs::deconstructMessage<int>(message);

            if (roomsHandler.isJoinPossible(roomNumber)) {
                roomsHandler.joinRoom(roomNumber, player);
                spdlog::info("{} joined room {}", player->name, roomNumber);
                sendInGameInfo();
            } else {
                sendSimpleRespond(chs::MessageType::ERROR_RESPOND);
                spdlog::warn("{} failed to join room {}: room not found", playerName, roomNumber);
            }

        }
            break;
        case chs::MessageType::QUIT_ROOM_REQUEST: {
            safelyQuitRoom();
        }
            break;
        case chs::MessageType::START_GAME_REQUEST: {
            auto &room = player->getRoom();

            if (room.getOwner() == player and room.getNumberOfPlayers() >= 2) {
               startNewRound();
                //TODO setup timer callbacks
            }
        }
            break;
        case chs::MessageType::ENTER_DRAWING_QUEUE_REQUEST: {
            auto position = player->getRoom().getInDrawingQueue(player);
            auto respondMessage = chs::constructMessage(chs::MessageType::SERVER_MESSAGE,
                                                        fmt::format("You are {} in queue", position));
            ioHandler.putMessage(socket, respondMessage);
            spdlog::info("{} entered drawing queue as {}", playerName, position);
        }
            break;
        case chs::MessageType::QUIT_DRAWING_QUEUE_REQUEST: {
            player->getRoom().quitDrawingQueue(player);
            spdlog::info("{} left drawing queue", playerName);
        }
            break;
        case chs::MessageType::CHAT_MESSAGE: {
            auto &room = player->getRoom();

            if (room.isGameActive() and room.getDrawer() != player) {
                auto players = room.getPlayers();

                auto [chat] = chs::deconstructMessage<std::string>(message);
                forwardChatMessage(chat);
                spdlog::info("Chat message from {}",  playerName);

                if (room.guessIsRight(chat)) {
                    player->addScore(chat.size());
                    room.getDrawer()->addScore(chat.size() / 2);
                    //TODO setup timer callbacks
                    //TODO check if someone won
                    sendSuccessfulGuess(chat);
                    startNewRound();

                } else if (room.guessIsClose(chat)) {
                    sendCloseGuess(chat);
                }
            }
        }
            break;
        case chs::MessageType::DRAW_LINE: {
            if (player->getRoom().getDrawer() == player) {
                auto players = player->getRoom().getPlayersButOne(player);
                auto renewedMessage = chs::addSizeToMessage(message);
                ioHandler.putMessage(players, renewedMessage);
            }
        }
            break;
        case chs::MessageType::CLEAR_DRAWING: {
            if (player->getRoom().getDrawer() == player) {
                auto players = player->getRoom().getPlayersButOne(player);
                auto renewedMessage = chs::addSizeToMessage(message);
                ioHandler.putMessage(players, renewedMessage);
            }
        }
            break;
        default:
            spdlog::error("Unsupported message type {}", chs::getMessageType(message));
            break;
    }
}

void LogicHandler::sendInGameInfo() {
    sendInGameInfo(player->getRoom());
}

void LogicHandler::sendSimpleRespond(chs::MessageType type) {
    sendSimpleRespond(socket, type);
}

void LogicHandler::safelyQuitRoom() {
    safelyQuitRoom(player);
}

void LogicHandler::safelyQuitRoom(Player *player) {
    if (player->isInRoom()) {
        auto roomNumber = player->getRoom().getRoomNumber();
        auto& roomsHandler = RoomsHandler::getInstance();

        spdlog::info("Player {} quit room {}", player->name, roomNumber);
        roomsHandler.quitRoom(player);

        if (roomsHandler.roomExists(roomNumber)) {
            sendInGameInfo(roomsHandler.getRoomByNumber(roomNumber));
        }
    }
}

void LogicHandler::sendInGameInfo(const Room &room) {
    auto inGameInfoMessage = room.getInGameInfo();
    IOHandler::getInstance().putMessage(room.getPlayers(), inGameInfoMessage);
}

void LogicHandler::forwardChatMessage(const std::string& chat) {
    auto newChatMessage = chs::constructMessage(chs::MessageType::CHAT_MESSAGE,
                                                fmt::format("{} : {}", playerName, chat));
    ioHandler.putMessage(player->getRoom().getPlayers(), newChatMessage);
}

void LogicHandler::startNewRound() {
    auto &room = player->getRoom();

    room.startRound();
    sendInGameInfo();

    auto clearDrawingMessage = chs::constructMessage(chs::MessageType::CLEAR_DRAWING);
    ioHandler.putMessage(room.getPlayers(), clearDrawingMessage);

    auto newRoundWordMessage = room.getCharadesWordMessage();
    ioHandler.putMessage(room.getDrawer()->getSocket(), newRoundWordMessage);
}

void LogicHandler::sendSuccessfulGuess(const std::string &guess) {
    auto guessIsRightMessage = chs::constructMessage(chs::MessageType::SERVER_MESSAGE,
                                                     fmt::format("{} SUCCEEDED! {}", playerName, guess));
    ioHandler.putMessage(player->getRoom().getPlayers(), guessIsRightMessage);
}

void LogicHandler::sendCloseGuess(const std::string & guess) {
    auto guessIsCloseMessage = chs::constructMessage(chs::MessageType::SERVER_MESSAGE,
                                                     fmt::format("CLOSE ONE! {}", guess));
    ioHandler.putMessage(player->getRoom().getPlayers(), guessIsCloseMessage);
}

void LogicHandler::sendSimpleRespond(chs::Socket socket, chs::MessageType type) {
    auto respondMessage = chs::constructMessage(type);
    IOHandler::getInstance().putMessage(socket, respondMessage);
}

