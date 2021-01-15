//
// Created by hubert on 30.12.2020.
//

#include "Room.hpp"

void Room::addPlayer(Player* player) {
    players.push_back(player);
    player->enterRoom(*this);
}

void Room::removePlayer(Player* player) {
    player->exitRoom();
    players.erase(std::remove(players.begin(), players.end(), player), players.end());
}

chs::Message Room::GetRoomInfo() {
    std::vector<std::string> playerNames;
    auto getPlayerName = [](const Player * player){ return player->name; };
    std::transform(players.begin(), players.end(), std::back_inserter(playerNames), getPlayerName);
    auto joinedNames = chs::joinStrings(playerNames, ";");
    return chs::constructMessage(chs::MessageType::ROOM_INFO, roomNumber, joinedNames);
}

Room::Room(int roomNumber) : roomNumber(roomNumber) {
}

int Room::getRoomNumber() {
    return roomNumber;
}

void Room::setOwner(Player* player) {
    owner = player;
}


