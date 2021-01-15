//
// Created by hubert on 29.12.2020.
//

#ifndef CHARADES_IOHANDLER_HPP
#define CHARADES_IOHANDLER_HPP

#include <map>
#include <Socket.hpp>
#include <IncomingMessageQueue.hpp>
#include <OutgoingMessageQueue.hpp>
#include "ConnectionHandler.hpp"

class IOHandler {
private:
    std::map<int, chs::IncomingMessageQueue> incomingQueues;
    std::map<int, chs::OutgoingMessageQueue> outgoingQueues;
    ConnectionHandler& connectionHandler;

public:

    IOHandler(ConnectionHandler& connectionHandler);
    void addClient(const chs::Socket & socket);
    void removeClient(const chs::Socket & socket);
    void receiveFrom(const chs::Socket & socket);
    void sendTo(const chs::Socket & socket);
    void putMessage(const chs::Socket & socket, const chs::Message & message);
    void putMessages(const chs::Socket & socket, std::vector<chs::Message> & messages);
    bool isMessageToGet(const chs::Socket & socket);
    chs::Message getMessage(const chs::Socket & socket);
};

#endif //CHARADES_IOHANDLER_HPP