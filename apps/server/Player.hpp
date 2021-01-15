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
    chs::Socket webSocket;
    Room * currentRoom;

public:
    Player(std::string name, const chs::Socket& webSocket);
    void enterRoom(Room & room);
    void exitRoom();
};

#endif //CHARADES_PLAYER_HPP