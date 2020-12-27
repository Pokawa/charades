//
// Created by hubert on 27.12.2020.
//

#ifndef CHARADES_PLAYER_HPP
#define CHARADES_PLAYER_HPP


#include <string>
#include <WebSocket.hpp>

class Player {
public:
    std::string name;
    chs::WebSocket webSocket;

public:
    Player(std::string name, const chs::WebSocket& webSocket);
};


#endif //CHARADES_PLAYER_HPP
