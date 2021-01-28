//
// Created by hubert on 29.12.2020.
//

#ifndef CHARADES_IOHANDLER_HPP
#define CHARADES_IOHANDLER_HPP

#include <map>
#include <Socket.hpp>
#include <IncomingMessageQueue.hpp>
#include <OutgoingMessageQueue.hpp>
#include <cpptime.h>
#include "ConnectionHandler.hpp"
#include "Player.hpp"

class IOHandler {
private:
    std::map<int, chs::IncomingMessageQueue> incomingQueues;
    std::map<int, chs::OutgoingMessageQueue> outgoingQueues;

    static std::unique_ptr<IOHandler> instance;
    static CppTime::Timer timer;

public:
    void operator=(const IOHandler &) = delete;
    IOHandler(IOHandler &other) = delete;
    IOHandler() = default;
    static IOHandler& getInstance();

    void addClient(const chs::Socket & socket);
    void removeClient(const chs::Socket & socket);
    void receiveFrom(chs::Socket & socket);
    void sendTo(chs::Socket socket);
    void putMessage(chs::Socket socket, const chs::Message & message);
    void putMessage(const std::vector<Player*>& players, const chs::Message & message);
    void putMessages(const chs::Socket & socket, std::vector<chs::Message> & messages);
    bool isMessageToGet(const chs::Socket & socket);
    chs::Message getMessage(const chs::Socket & socket);

    void closeConnection(chs::Socket &socket);
};

#endif //CHARADES_IOHANDLER_HPP