//
// Created by hubert on 29.12.2020.
//

#include "IOHandler.hpp"
#include "LogicHandler.hpp"

CppTime::Timer IOHandler::timer;

void IOHandler::addClient(const chs::Socket &socket) {
    incomingQueues.emplace(socket.getDescriptor(), socket);
    outgoingQueues.emplace(socket.getDescriptor(), socket);
}

void IOHandler::removeClient(const chs::Socket &socket) {
    incomingQueues.erase(socket.getDescriptor());
    outgoingQueues.erase(socket.getDescriptor());
}

void IOHandler::receiveFrom(chs::Socket &socket) {
    auto success = incomingQueues.at(socket.getDescriptor()).readMessages();

    if (not success) {
        spdlog::error("error while reading from {}", socket.getPort());
        closeConnection(socket);
    }
}

void IOHandler::closeConnection(chs::Socket &socket) {
    if (PlayersHandler::getInstance().clientIsLoggedIn(socket)) {
        auto& player = PlayersHandler::getInstance().getPlayer(socket);
        LogicHandler::safelyQuitRoom(&player);
        PlayersHandler::getInstance().removePlayer(socket);
    }
    removeClient(socket);
    ConnectionHandler::getInstance().closeClient(socket);
}

void IOHandler::sendTo(chs::Socket socket) {
    auto& queue = outgoingQueues.at(socket.getDescriptor());
    bool success = queue.sendMessages();

    if (not success) {
        spdlog::error("error while sending to {}", socket.getPort());
        closeConnection(socket);
        return;
    }

    if (queue.isBlocked()) {
        ConnectionHandler::getInstance().setForWrite(socket);

        spdlog::warn("{} is blocked", socket.getPort());

        timer.add(std::chrono::seconds(30), [&queue, &socket](CppTime::timer_id id){
            if (queue.isBlocked()) {
                spdlog::error("{} is still blocked", socket.getPort());
                IOHandler::getInstance().closeConnection(socket);
            }
        });

    } else {
        ConnectionHandler::getInstance().unsetForWrite(socket);
    }
}

void IOHandler::putMessage(chs::Socket socket, const chs::Message & message) {
    outgoingQueues.at(socket.getDescriptor()).putMessage(message);
    sendTo(socket);
}

chs::Message IOHandler::getMessage(const chs::Socket &socket) {
    return incomingQueues.at(socket.getDescriptor()).getMessage();
}

bool IOHandler::isMessageToGet(const chs::Socket &socket) {
    return incomingQueues.at(socket.getDescriptor()).isNotEmpty();
}

void IOHandler::putMessages(const chs::Socket &socket, std::vector<chs::Message> & messages) {
    auto& queue = outgoingQueues.at(socket.getDescriptor());
    auto putMessage = [&queue](chs::Message & message){ queue.putMessage(std::move(message)); };
    std::for_each(messages.begin(), messages.end(), putMessage);
    ConnectionHandler::getInstance().setForWrite(socket);
}

std::unique_ptr<IOHandler> IOHandler::instance;

IOHandler &IOHandler::getInstance() {
    if (instance == nullptr) {
        instance = std::make_unique<IOHandler>();
    }
    return *instance;
}

void IOHandler::putMessage(const std::vector<Player *>& players, const chs::Message & message) {
    for (auto* player : players) {
        putMessage(player->getSocket(), message);
    }
}



