//
// Created by hubert on 28.01.2021.
//

#ifndef CHARADES_LOGICHANDLER_HPP
#define CHARADES_LOGICHANDLER_HPP


#include "Player.hpp"
#include "RoomsHandler.hpp"
#include "PlayersHandler.hpp"
#include "IOHandler.hpp"

class LogicHandler {
    Player * player;
    std::string playerName;
    chs::Socket socket;
    RoomsHandler& roomsHandler;
    IOHandler& ioHandler;
    PlayersHandler& playersHandler;
    ConnectionHandler& connectionHandler;

public:
    static CppTime::Timer timer;

public:
    explicit LogicHandler(Player * player);
    void handleMessage(chs::Message message);
    static void sendInGameInfo(const Room & room);
    static void safelyQuitRoom(Player* player);
    static void sendSimpleRespond(chs::Socket socket, chs::MessageType type);
    static void startNewRound(Room& room);
    static void setTimerCallbacks(Room& room);
    static void sendServerMessage(Room& room, const std::string& message);

private:
    void sendInGameInfo();
    void sendServerMessage(const std::string& message);
    void sendSuccessfulGuess(const std::string& guess);
    void sendCloseGuess(const std::string& guess);
    void sendSimpleRespond(chs::MessageType type);
    void safelyQuitRoom();
    void forwardChatMessage(const std::string& chat);
};


#endif //CHARADES_LOGICHANDLER_HPP
