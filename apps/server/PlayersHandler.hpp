//
// Created by hubert on 27.12.2020.
//

#ifndef CHARADES_PLAYERSHANDLER_HPP
#define CHARADES_PLAYERSHANDLER_HPP

#include <Socket.hpp>
#include <vector>
#include <memory>
#include <poll.h>
#include "ConnectionHandler.hpp"
#include "Player.hpp"

class PlayersHandler {
private:
    std::vector<Player> players;

public:
    void addPlayer(const std::string& name, const chs::Socket& webSocket);
    Player& getPlayer(const chs::Socket & socket);
    bool isNameAvailable(const std::string& name);
};


#endif //CHARADES_PLAYERSHANDLER_HPP
