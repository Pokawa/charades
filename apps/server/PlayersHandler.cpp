//
// Created by hubert on 27.12.2020.
//

#include "PlayersHandler.hpp"
#include <algorithm>

void PlayersHandler::addPlayer(const std::string &name, const chs::WebSocket& webSocket) {
    players.emplace_back(name, webSocket);
}

bool PlayersHandler::isNameAvailable(const std::string &name) {
    auto position = std::find_if(players.begin(), players.end(),
                                 [&name](const Player & player){ return player.name == name; });
    return position == players.end();
}
