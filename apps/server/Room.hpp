//
// Created by hubert on 30.12.2020.
//

#ifndef CHARADES_ROOM_HPP
#define CHARADES_ROOM_HPP

#include <vector>
#include <functional>
#include <Message.hpp>
#include "Player.hpp"

class Room {
private:
    std::vector<Player*> players;
    int roomNumber;
    Player * owner;

public:
    explicit Room(int roomNumber);
    void addPlayer(Player* player);
    void removePlayer(Player* player);
    chs::Message GetRoomInfo();
    int getRoomNumber();
    void setOwner(Player* player);
};

#endif //CHARADES_ROOM_HPP
