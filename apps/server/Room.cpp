//
// Created by hubert on 30.12.2020.
//

#include "Room.hpp"
#include "CharadesWordsPool.hpp"
#include "IOHandler.hpp"
#include "LogicHandler.hpp"

void Room::addPlayer(Player* player) {
    player->setScore(0);
    players.push_back(player);
    player->enterRoom(*this);
}

void Room::removePlayer(Player* player) {
    player->exitRoom();
    players.erase(std::remove(players.begin(), players.end(), player), players.end());
    quitDrawingQueue(player);

    if (player == owner and not players.empty()) {
        setOwner(players.front());
    }

    if (player == drawer and drawingQueue.empty()) {
        drawer = owner;
    }

    if (getNumberOfPlayers() < 2) {
        gameIsActive = false;
    }
}

chs::Message Room::getRoomInfo() const {
    auto joinedNames = getJoinedPLayerNames();
    return chs::constructMessage(chs::MessageType::ROOM_INFO_RESPOND, roomNumber, gameIsActive, joinedNames);
}

chs::Message Room::getInGameInfo() const {
    auto joinedNames = getJoinedPLayerNames();
    auto joinedScores = getJoinedPlayerScores();
    return chs::constructMessage(chs::MessageType::IN_GAME_INFO_RESPOND, owner->name, joinedNames, joinedScores, gameIsActive, roundStartTimePoint, drawer->name, wordCount);
}

Room::Room(int roomNumber, Player* owner) : roomNumber(roomNumber),
                                            owner(owner),
                                            drawer(owner),
                                            gameIsActive(false),
                                            roundStartTimePoint(),
                                            wordCount(0) {
    addPlayer(owner);
    endOfRoundTimer = halfTheTimeTimer = threeQuartersTimeTimer = -1;
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
    drawingQueue.erase(std::remove(drawingQueue.begin(), drawingQueue.end(), player), drawingQueue.end());
}

void Room::nextDrawer() {
    if (not drawingQueue.empty()) {
        drawer = drawingQueue.front();
        drawingQueue.push_back(drawer);
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

Player *Room::getOwner() const {
    return owner;
}

void Room::stopTimers(CppTime::Timer& timer) {
    timer.remove(endOfRoundTimer);
    timer.remove(halfTheTimeTimer);
    timer.remove(threeQuartersTimeTimer);
}

void Room::setTheTimers(CppTime::timer_id endTimer, CppTime::timer_id halfTimer,
                        CppTime::timer_id quarterTimer) {
    endOfRoundTimer = endTimer;
    halfTheTimeTimer = halfTimer;
    threeQuartersTimeTimer = quarterTimer;
}

std::string Room::getCharadesWord() const {
    return charadesWord;
}

void Room::stopGame() {
    gameIsActive = false;
    for (auto* roomMember : players) {
        roomMember->setScore(0);
    }
    stopTimers(LogicHandler::timer);
}

void Room::setEndTimerAsInvoked() {
    endOfRoundTimer = -1;
}

void Room::setHalfTimerAsInvoked() {
    halfTheTimeTimer = -1;
}

void Room::setQuarterTimerAsInvoked() {
    threeQuartersTimeTimer = -1;
}


