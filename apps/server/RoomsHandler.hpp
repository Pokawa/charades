//
// Created by hubert on 15.01.2021.
//

#ifndef CHARADES_ROOMSHANDLER_HPP
#define CHARADES_ROOMSHANDLER_HPP


#include <vector>
#include "Room.hpp"

class RoomsHandler {
private:
    std::vector<Room> rooms;
    int latestRoomNumber;

public:
    RoomsHandler();
    void joinRoom(int roomNumber, Player* player);
    void quitRoom(int roomNumber, Player* player);
    void newRoom(Player* owner);
    std::vector<chs::Message> getRoomsInfo();
    Room& getRoomByNumber(int roomNumber);
    bool roomExists(int roomNumber);

private:
    std::vector<Room>::iterator findRoomByNumber(int roomNumber);
};


#endif //CHARADES_ROOMSHANDLER_HPP
