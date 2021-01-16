//
// Created by hubert on 15.01.2021.
//

#include "RoomsHandler.hpp"

void RoomsHandler::newRoom(Player *owner) {
    latestRoomNumber += 10;
    rooms.emplace_back(latestRoomNumber);
    rooms.back().addPlayer(owner);
    rooms.back().setOwner(owner);
}

std::vector<chs::Message> RoomsHandler::getRoomsInfo() {
    std::vector<chs::Message> roomsInfo;
    auto getRoomInfo = [](const Room & room){ return room.getRoomInfo(); };
    std::transform(rooms.begin(), rooms.end(), std::back_inserter(roomsInfo), getRoomInfo);
    return roomsInfo;
}

RoomsHandler::RoomsHandler() {
    latestRoomNumber = 1;
    rooms.emplace_back(latestRoomNumber);
}
