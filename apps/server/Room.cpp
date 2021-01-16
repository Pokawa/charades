//
// Created by hubert on 30.12.2020.
//

#include "Room.hpp"

void Room::addPlayer(Player* player) {
    players.push_back(player);
    player->enterRoom(*this);
    refreshRoomInfo();
}

void Room::removePlayer(Player* player) {
    player->exitRoom();
    players.erase(std::remove(players.begin(), players.end(), player), players.end());
    refreshRoomInfo();
}

chs::Message Room::getRoomInfo() const {
    return roomInfo;
}

void Room::refreshRoomInfo() {
    std::vector<std::string> playerNames;
    auto getPlayerName = [](const Player * player){ return player->name; };
    std::transform(players.begin(), players.end(), std::back_inserter(playerNames), getPlayerName);
    auto joinedNames = chs::joinStrings(playerNames, ";");
    roomInfo = chs::constructMessage(chs::MessageType::ROOM_INFO_RESPOND, roomNumber, joinedNames);
}

Room::Room(int roomNumber) : roomNumber(roomNumber), owner(nullptr) {
    refreshRoomInfo();
}

int Room::getRoomNumber() const {
    return roomNumber;
}

void Room::setOwner(Player* player) {
    owner = player;
}


