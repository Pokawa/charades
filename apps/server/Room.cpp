//
// Created by hubert on 30.12.2020.
//

#include "Room.hpp"
#include "CharadesWordsPool.hpp"

void Room::addPlayer(Player* player) {
    players.push_back(player);
    player->enterRoom(*this);
}

void Room::removePlayer(Player* player) {
    player->exitRoom();
    players.erase(std::remove(players.begin(), players.end(), player), players.end());
    drawingQueue.erase(std::remove(drawingQueue.begin(), drawingQueue.end(), player), drawingQueue.end());

    if (player == owner and not players.empty()) {
        setOwner(players.front());
    }

    if (player == drawer) {
        if (drawingQueue.empty()) {
            drawer = owner;
        } else {
            nextDrawer();
        }
    }

    if (getNumberOfPlayers() < 2) {
        gameIsActive = false;
    }
}

chs::Message Room::getRoomInfo() const {
    auto joinedNames = getJoinedPLayerNames();
    return chs::constructMessage(chs::MessageType::ROOM_INFO_RESPOND, roomNumber, joinedNames);
}

chs::Message Room::getInGameInfo() const {
    auto joinedNames = getJoinedPLayerNames();
    auto joinedScores = getJoinedPlayerScores();
    return chs::constructMessage(chs::MessageType::IN_GAME_INFO_RESPOND, owner->name, joinedNames, joinedScores, gameIsActive, roundStartTimePoint, drawer->name, wordCount);
}

Room::Room(int roomNumber, Player* owner) : roomNumber(roomNumber), owner(owner), drawer(owner), gameIsActive(false), roundStartTimePoint(), wordCount(0) {
    addPlayer(owner);
}

int Room::getRoomNumber() const {
    return roomNumber;
}

void Room::setOwner(Player* player) {
    owner = player;
}

int Room::getNumberOfPlayers() const {
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
       drawingQueue.push_back(drawer);
       drawer = drawingQueue.front();
       drawingQueue.pop_front();
    }
}

std::vector<Player *> Room::getPlayersButOne(Player * player) const {
    std::vector<Player *> buffer(players.begin(), players.end());
    buffer.erase(std::find(buffer.begin(), buffer.end(), player));
    return buffer;
}

std::vector<Player *> Room::getPlayers() const {
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

void Room::startRound() {
    roundStartTimePoint = std::chrono::system_clock::now();
    gameIsActive = true;
    charadesWord = CharadesWordsPool::getInstance().getRandomWord();
    wordCount = static_cast<int>(std::count(charadesWord.begin(), charadesWord.end(), ' ')) + 1;
    nextDrawer();
}

Player *Room::getDrawer() const {
    return drawer;
}

std::string Room::getCharadesWordMessage() const {
    return chs::constructMessage(chs::MessageType::CHARADES_WORD_RESPOND, charadesWord);
}

bool Room::guessIsClose(const std::string& guess) const {
    return guess.length() >= 3 and charadesWord.find(guess) != std::string::npos;
}

bool Room::guessIsRight(const std::string& guess) const {
    return charadesWord == guess;
}

bool Room::isGameActive() const {
    return gameIsActive;
}

Player *Room::getOwner() {
    return owner;
}


