//
// Created by hubert on 28.01.2021.
//

#include <Message.hpp>
#include "LogicHandler.hpp"

CppTime::Timer LogicHandler::timer;

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
                spdlog::info("{} joined room {}", playerName, roomNumber);
                sendServerMessage(fmt::format("{} joined room", playerName));
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
                startNewRound(room);
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

                auto[chat] = chs::deconstructMessage<std::string>(message);
                forwardChatMessage(chat);
                spdlog::info("Chat message from {}", playerName);

                if (room.guessIsRight(chat)) {
                    player->addScore(chat.size());
                    room.getDrawer()->addScore(static_cast<int>(chat.size()) / 2);
                    sendSuccessfulGuess(chat);
                    startNewRound(room);
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
        auto &roomsHandler = RoomsHandler::getInstance();

        spdlog::info("Player {} quit room {}", player->name, roomNumber);
        roomsHandler.quitRoom(player);

        if (roomsHandler.roomExists(roomNumber)) {
            auto &room = roomsHandler.getRoomByNumber(roomNumber);
            sendInGameInfo(room);
            sendServerMessage(room, fmt::format("{} left the room", player->name));
        }
    }
}

void LogicHandler::sendInGameInfo(const Room &room) {
    auto inGameInfoMessage = room.getInGameInfo();
    IOHandler::getInstance().putMessage(room.getPlayers(), inGameInfoMessage);
}

void LogicHandler::forwardChatMessage(const std::string &chat) {
    auto newChatMessage = chs::constructMessage(chs::MessageType::CHAT_MESSAGE,
                                                fmt::format("{} : {}", playerName, chat));
    ioHandler.putMessage(player->getRoom().getPlayers(), newChatMessage);
}

void LogicHandler::startNewRound(Room &room) {
    room.startRound();
    room.stopTimers(timer);
    setTimerCallbacks(room);
    sendInGameInfo(room);

    auto clearDrawingMessage = chs::constructMessage(chs::MessageType::CLEAR_DRAWING);
    IOHandler::getInstance().putMessage(room.getPlayers(), clearDrawingMessage);

    auto newRoundWordMessage = room.getCharadesWordMessage();
    IOHandler::getInstance().putMessage(room.getDrawer()->getSocket(), newRoundWordMessage);
}

void LogicHandler::sendSuccessfulGuess(const std::string &guess) {
    sendServerMessage(fmt::format("{} SUCCEEDED! {}", playerName, guess));
}

void LogicHandler::sendCloseGuess(const std::string &guess) {
    sendServerMessage(fmt::format("CLOSE ONE! {}", guess));
}

void LogicHandler::sendSimpleRespond(chs::Socket socket, chs::MessageType type) {
    auto respondMessage = chs::constructMessage(type);
    IOHandler::getInstance().putMessage(socket, respondMessage);
}

void LogicHandler::setTimerCallbacks(Room &room) {
    auto endRoundCallback = [&room](CppTime::timer_id) {
        auto *drawer = room.getDrawer();
        auto charadesWord = room.getCharadesWord();
        drawer->setScore(static_cast<int>(charadesWord.size()) / -2);

        sendServerMessage(room, fmt::format("{} FAILED! Word was: {}", drawer->name, charadesWord));

        LogicHandler::startNewRound(room);
    };

    auto halfTimeCallback = [&room](CppTime::timer_id) {
        sendServerMessage(room, fmt::format("HINT: {}...", chs::utf8_substr(room.getCharadesWord(), 0, 1)));
    };

    auto threeQuarterTimeCallback = [&room](CppTime::timer_id) {
        sendServerMessage(room, fmt::format("HINT: {}...", chs::utf8_substr(room.getCharadesWord(), 0, 2)));
    };

    auto endOfRoundTimer = timer.add(std::chrono::minutes(3), endRoundCallback);
    auto halfTheTimeTimer = timer.add(std::chrono::seconds(90), halfTimeCallback);
    auto threeQuartersTimeTimer = timer.add(std::chrono::seconds(135), threeQuarterTimeCallback);

    room.setTheTimers(endOfRoundTimer, halfTheTimeTimer, threeQuartersTimeTimer);
}

void LogicHandler::sendServerMessage(const std::string &message) {
    sendServerMessage(player->getRoom(), message);
}

void LogicHandler::sendServerMessage(Room &room, const std::string &message) {
    auto serverMessage = chs::constructMessage(chs::MessageType::SERVER_MESSAGE, message);
    IOHandler::getInstance().putMessage(room.getPlayers(), serverMessage);
}

