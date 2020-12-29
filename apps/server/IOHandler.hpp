//
// Created by hubert on 29.12.2020.
//

#pragma once

#include <map>
#include <WebSocket.hpp>
#include <IncomingMessageQueue.hpp>
#include <OutgoingMessageQueue.hpp>

class IOHandler {
private:
    std::map<int, chs::IncomingMessageQueue> incomingQueues;
    std::map<int, chs::OutgoingMessageQueue> outgoingQueues;

public:

    void addClient(const chs::WebSocket & socket);
    void removeClient(const chs::WebSocket & socket);
    void receiveFrom(const chs::WebSocket & socket);
    void sendTo(const chs::WebSocket & socket);
    void putMessage(const chs::WebSocket & socket, const chs::Message & message);
    bool isMessageToGet(const chs::WebSocket & socket);
    chs::Message getMessage(const chs::WebSocket & socket);
};
