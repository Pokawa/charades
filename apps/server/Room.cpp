//
// Created by hubert on 30.12.2020.
//

#include "Room.hpp"

void Room::addPlayer(Player* player) {
    players.push_back(player);
    player->enterRoom(*this);
}

void Room::removePlayer(Player* player) {
    player->exitRoom();
    players.erase(std::remove(players.begin(), players.end(), player), players.end());

    if (player == owner and not players.empty()) {
        setOwner(players.front());
    }
}

chs::Message Room::getRoomInfo() const {
    auto joinedNames = getJoinedPLayerNames();
    return chs::constructMessage(chs::MessageType::ROOM_INFO_RESPOND, roomNumber, joinedNames);
}

chs::Message Room::getInGameInfo() const {
    auto joinedNames = getJoinedPLayerNames();
    auto joinedScores = getJoinedPlayerScores();
    return chs::constructMessage(chs::MessageType::IN_GAME_INFO_RESPOND, owner->name, joinedNames, joinedScores, gameIsActive, roundStartTimePoint, drawer->name);
}

Room::Room(int roomNumber) : Room(roomNumber, nullptr) {
}

Room::Room(int roomNumber, Player* owner) : roomNumber(roomNumber), owner(owner), drawer(owner), gameIsActive(false), roundStartTimePoint() {
    addPlayer(owner);
}

int Room::getRoomNumber() const {
    return roomNumber;
}

void Room::setOwner(Player* player) {
    owner = player;
}

int Room::getNumberOfPlayers() {
    return players.size();
}

int Room::getInDrawingQueue(Player *player) {
    drawingQueue.push_back(player);
    return drawingQueue.size();
}

void Room::quitDrawingQueue(Player *player) {
    drawingQueue.erase(std::find(drawingQueue.begin(), drawingQueue.end(), player));
}

void Room::nextDrawer() {
    if (not drawingQueue.empty()) {
        if (drawer == drawingQueue.front()) {
            drawingQueue.push_back(drawer);
            drawingQueue.pop_front();
        }

        drawer = drawingQueue.front();
    }
}

std::vector<Player *> Room::getPlayersButOne(Player * player) {
    std::vector<Player *> buffer(players.begin(), players.end());
    buffer.erase(std::find(buffer.begin(), buffer.end(), player));
    return buffer;
}

std::vector<Player *> Room::getPlayers() {
    std::vector<Player *> vec(players.begin(), players.end());
    return vec;
}

std::string Room::getJoinedPLayerNames() const {
    std::vector<std::string> playerNames;
    auto getPlayerName = [](const Player * player){ return player->name; };
    std::transform(players.begin(), players.end(), std::back_inserter(playerNames), getPlayerName);
    return chs::joinStrings(playerNames, ';');
}

std::string Room::getJoinedPlayerScores() const {
    std::vector<std::string> playerScores;
    auto getPlayerScore = [](const Player * player){ return std::to_string(player->getScore()); };
    std::transform(players.begin(), players.end(), std::back_inserter(playerScores), getPlayerScore);
    return chs::joinStrings(playerScores, ';');
}


