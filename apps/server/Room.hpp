//
// Created by hubert on 30.12.2020.
//

#ifndef CHARADES_ROOM_HPP
#define CHARADES_ROOM_HPP

#include <vector>
#include <functional>
#include <Message.hpp>
#include <list>
#include "Player.hpp"
#include "cpptime.h"

class Room {
private:
    std::list<Player*> players;
    std::list<Player*> drawingQueue;
    int roomNumber;
    Player * owner;
    Player * drawer;
    std::string charadesWord;
    int wordCount;
    bool gameIsActive;
    std::chrono::time_point<std::chrono::system_clock> roundStartTimePoint;

    CppTime::timer_id endOfRoundTimer;
    CppTime::timer_id halfTheTimeTimer;
    CppTime::timer_id threeQuartersTimeTimer;

public:
    Room(int roomNumber, Player * owner);
    void addPlayer(Player* player);
    void removePlayer(Player* player);
    [[nodiscard]] chs::Message getRoomInfo() const;
    [[nodiscard]] chs::Message getInGameInfo() const;
    [[nodiscard]] int getRoomNumber() const;
    void setOwner(Player* player);
    int getInDrawingQueue(Player * player);
    void quitDrawingQueue(Player * player);
    void nextDrawer();
    [[nodiscard]] int getNumberOfPlayers() const;
    [[nodiscard]] std::vector<Player*> getPlayersButOne(Player * player) const;
    [[nodiscard]] std::vector<Player *> getPlayers() const;
    void startRound();
    [[nodiscard]] Player* getDrawer() const;
    [[nodiscard]] std::string getCharadesWordMessage() const;
    [[nodiscard]] bool guessIsClose(const std::string& guess) const;
    [[nodiscard]] bool guessIsRight(const std::string& guess) const;
    [[nodiscard]] bool isGameActive() const;
    [[nodiscard]] Player *getOwner() const;
    [[nodiscard]] std::string getCharadesWord() const;
    void setTheTimers(CppTime::timer_id endOfRoundTimer, CppTime::timer_id halfTheTimeTimer, CppTime::timer_id threeQuartersTimeTimer);
    void stopTimers(CppTime::Timer& timer);
    void stopGame();

private:
    [[nodiscard]] std::string getJoinedPLayerNames() const;
    [[nodiscard]] std::string getJoinedPlayerScores() const;

};

#endif //CHARADES_ROOM_HPP
