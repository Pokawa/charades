//
// Created by hubert on 27.12.2020.
//

#include "Player.hpp"

#include <utility>

Player::Player(std::string name, const chs::WebSocket& webSocket) : name(std::move(name)), webSocket(webSocket){
}