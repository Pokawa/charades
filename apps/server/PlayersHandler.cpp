//
// Created by hubert on 27.12.2020.
//

#include "PlayersHandler.hpp"
#include "RoomsHandler.hpp"
#include <algorithm>

void PlayersHandler::addPlayer(const std::string &name, const chs::Socket& webSocket) {
    players.emplace_back(name, webSocket);
}

bool PlayersHandler::isNameAvailable(const std::string &name) {
    auto position = std::find_if(players.begin(), players.end(),
                                 [&name](const Player & player){ return player.name == name; });
    return position == players.end();
}

Player &PlayersHandler::getPlayer(const chs::Socket &socket) {
    return *getPlayerPosition(socket);
}

std::unique_ptr<PlayersHandler> PlayersHandler::instance;

PlayersHandler &PlayersHandler::getInstance() {
    if (instance == nullptr) {
        instance = std::make_unique<PlayersHandler>();
    }
    return *instance;
}

void PlayersHandler::removePlayer(const chs::Socket & socket) {
    auto position = getPlayerPosition(socket);
    if (position != players.end())
        players.erase(position);
}

std::list<Player>::iterator PlayersHandler::getPlayerPosition(const chs::Socket & socket) {
    return std::find_if(players.begin(), players.end(),
                                 [&socket](const Player & player){ return player.compareSocket(socket); });
}

bool PlayersHandler::clientIsLoggedIn(const chs::Socket &socket) {
    return getPlayerPosition(socket) != players.end();
}

std::vector<Player *> PlayersHandler::getPlayers() {
    std::vector<Player*> vec;
    auto getPlayerAddr = [](Player& player){ return &player; };
    std::transform(players.begin(), players.end(), std::back_inserter(vec), getPlayerAddr);
    return vec;
}

bool PlayersHandler::isNameValid(const std::string &name) {
    return not name.empty() and name.find(';') == std::string::npos and isNameAvailable(name);
}

