//
// Created by hubert on 30.12.2020.
//

#include "Room.hpp"

void Room::addPlayer(Player &player) {
    players.emplace_back(player);
    player.enterRoom(*this);
}

void Room::removePlayer(Player &player) {
    auto position = std::find_if(players.begin(), players.end(),
                 [player](const std::reference_wrapper<Player> & item){ return item.get().name == player.name; });
    if (position != players.end()) {
        players.erase(position);
        player.exitRoom();
    }
}


