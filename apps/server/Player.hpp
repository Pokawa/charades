//
// Created by hubert on 27.12.2020.
//

#ifndef CHARADES_PLAYER_HPP
#define CHARADES_PLAYER_HPP

#include <string>
#include <Socket.hpp>

class Room;

class Player {
public:
    std::string name;

private:
    chs::Socket socket;
    Room * currentRoom;
    int score;

public:
    Player(std::string name, const chs::Socket& socket);
    void enterRoom(Room & room);
    [[nodiscard]] bool compareSocket(const chs::Socket & comparingSocket) const;
    Room& getRoom();
    [[nodiscard]] chs::Socket getSocket() const;
    bool isInRoom();
    void exitRoom();
    void setScore(int newScore);
    void addScore(int increment);
    [[nodiscard]] int getScore() const;

};

#endif //CHARADES_PLAYER_HPP