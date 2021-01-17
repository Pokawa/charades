//
// Created by hubert on 27.12.2020.
//

#include "Player.hpp"
#include <utility>

Player::Player(std::string name, const chs::Socket& socket) : name(std::move(name)), socket(socket){
}

void Player::enterRoom(Room & newRoom) {
    currentRoom = &newRoom;
}

void Player::exitRoom() {
    currentRoom = nullptr;
}

bool Player::compareSocket(const chs::Socket & comparingSocket) const {
    return this->socket.getDescriptor() == comparingSocket.getDescriptor();
}

Room &Player::getRoom() {
    return *currentRoom;
}
