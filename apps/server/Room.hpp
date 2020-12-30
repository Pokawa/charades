//
// Created by hubert on 30.12.2020.
//

#ifndef CHARADES_ROOM_HPP
#define CHARADES_ROOM_HPP

#include <vector>
#include <functional>
#include "Player.hpp"

class Room {
private:
    std::vector<std::reference_wrapper<Player>> players;

public:
    void addPlayer(Player & player);
    void removePlayer(Player & player);
};

#endif //CHARADES_ROOM_HPP
