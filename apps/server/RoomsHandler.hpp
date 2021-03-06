//
// Created by hubert on 15.01.2021.
//

#ifndef CHARADES_ROOMSHANDLER_HPP
#define CHARADES_ROOMSHANDLER_HPP


#include <list>
#include "Room.hpp"

class RoomsHandler {
private:
    std::list<Room> rooms;
    int latestRoomNumber;

    static std::unique_ptr<RoomsHandler> instance;

public:
    void operator=(const RoomsHandler &) = delete;
    RoomsHandler(RoomsHandler &other) = delete;
    static RoomsHandler& getInstance();

    RoomsHandler();
    void joinRoom(int roomNumber, Player* player);
    void quitRoom(Player* player);
    void newRoom(Player* owner);
    std::vector<chs::Message> getRoomsInfo();
    Room& getRoomByNumber(int roomNumber);
    bool roomExists(int roomNumber);
    bool isJoinPossible(int roomNumber);

private:
    std::list<Room>::iterator findRoomByNumber(int roomNumber);
};


#endif //CHARADES_ROOMSHANDLER_HPP
