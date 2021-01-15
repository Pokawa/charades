//
// Created by hubert on 15.01.2021.
//

#include "RoomsHandler.hpp"

void RoomsHandler::newRoom(Player *owner) {
    auto newRoomNumber = rooms.back().getRoomNumber() + 10;
    rooms.emplace_back(newRoomNumber);
    rooms.back().addPlayer(owner);
    rooms.back().setOwner(owner);
}

std::vector<chs::Message> RoomsHandler::getRoomsInfo() {
    std::vector<chs::Message> roomsInfo;
    auto getRoomInfo = [](Room * room){ return room->GetRoomInfo(); };
    std::transform(rooms.begin(), rooms.end(), std::back_inserter(roomsInfo), getRoomInfo);
    return roomsInfo;
}
