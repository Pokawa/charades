//
// Created by hubert on 27.12.2020.
//

#include "Player.hpp"
#include <utility>

Player::Player(std::string name, const chs::Socket& webSocket) : name(std::move(name)), webSocket(webSocket){
}

void Player::enterRoom(Room & newRoom) {
    currentRoom = &newRoom;
}

void Player::exitRoom() {
    currentRoom = nullptr;
}
