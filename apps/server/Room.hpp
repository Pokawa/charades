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
    chs::Message roomInfo;

public:
    explicit Room(int roomNumber);
    void addPlayer(Player* player);
    void removePlayer(Player* player);
    [[nodiscard]] chs::Message getRoomInfo() const;
    [[nodiscard]] int getRoomNumber() const;
    void setOwner(Player* player);

    int getNumberOfPlayers();

private:
    void refreshRoomInfo();
};

#endif //CHARADES_ROOM_HPP
