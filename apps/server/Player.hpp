//
// Created by hubert on 27.12.2020.
//

#ifndef CHARADES_PLAYER_HPP
#define CHARADES_PLAYER_HPP

#include <string>
#include <WebSocket.hpp>

class Room;

class Player {
public:
    std::string name;

private:
    chs::WebSocket webSocket;
    Room * currentRoom;

public:
    Player(std::string name, const chs::WebSocket& webSocket);
    void enterRoom(Room & room);
    void exitRoom();
};

#endif //CHARADES_PLAYER_HPP