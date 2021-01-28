//
// Created by hubert on 15.01.2021.
//

#include "RoomsHandler.hpp"

void RoomsHandler::newRoom(Player *owner) {
    latestRoomNumber += 10;
    rooms.emplace_back(latestRoomNumber, owner);
}

std::vector<chs::Message> RoomsHandler::getRoomsInfo() {
    std::vector<chs::Message> roomsInfo;
    auto getRoomInfo = [](const Room & room){ return room.getRoomInfo(); };
    std::transform(rooms.begin(), rooms.end(), std::back_inserter(roomsInfo), getRoomInfo);
    return roomsInfo;
}

RoomsHandler::RoomsHandler() = default;

void RoomsHandler::joinRoom(int roomNumber, Player* player) {
    getRoomByNumber(roomNumber).addPlayer(player);
}

void RoomsHandler::quitRoom(Player *player) {
    if (player->isInRoom()) {
        auto& room = player->getRoom();
        room.removePlayer(player);

        if (room.getNumberOfPlayers() == 0) {
            spdlog::info("Removed room {}", room.getRoomNumber());
            rooms.erase(findRoomByNumber(room.getRoomNumber()));
        }
    }
}

Room &RoomsHandler::getRoomByNumber(int roomNumber) {
    return *findRoomByNumber(roomNumber);
}

std::vector<Room>::iterator RoomsHandler::findRoomByNumber(int roomNumber) {
    auto isSameRoomNumber = [roomNumber](const Room& room){ return room.getRoomNumber() == roomNumber; };
    return std::find_if(rooms.begin(), rooms.end(), isSameRoomNumber);
}

bool RoomsHandler::roomExists(int roomNumber) {
    return findRoomByNumber(roomNumber) != rooms.end();
}

std::unique_ptr<RoomsHandler> RoomsHandler::instance;

RoomsHandler &RoomsHandler::getInstance() {
    if (instance == nullptr) {
        instance = std::make_unique<RoomsHandler>();
    }
    return *instance;
}

bool RoomsHandler::isJoinPossible(int roomNumber) {
    return roomExists(roomNumber) and getRoomByNumber(roomNumber).isGameActive();
}
