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

public:
    void newRoom(Player* owner);
    std::vector<chs::Message> getRoomsInfo();
};


#endif //CHARADES_ROOMSHANDLER_HPP
