//
// Created by hubert on 27.12.2020.
//

#ifndef CHARADES_PLAYERSHANDLER_HPP
#define CHARADES_PLAYERSHANDLER_HPP

#include <Socket.hpp>
#include <list>
#include <memory>
#include <poll.h>
#include "ConnectionHandler.hpp"
#include "Player.hpp"

class PlayersHandler {
private:
    std::list<Player> players;

    static std::unique_ptr<PlayersHandler> instance;

public:
    void operator=(const PlayersHandler &) = delete;
    PlayersHandler(PlayersHandler &other) = delete;
    PlayersHandler() = default;
    static PlayersHandler& getInstance();


    std::vector<Player*> getPlayers();
    void addPlayer(const std::string& name, const chs::Socket& webSocket);
    void removePlayer(const chs::Socket & socket);
    bool clientIsLoggedIn(const chs::Socket & socket);
    Player& getPlayer(const chs::Socket & socket);
    bool isNameValid(const std::string& name);
    bool isNameAvailable(const std::string& name);

private:
    std::list<Player>::iterator getPlayerPosition(const chs::Socket & socket);
};


#endif //CHARADES_PLAYERSHANDLER_HPP
