//
// Created by hubert on 29.12.2020.
//

#include "OutgoingMessageQueue.hpp"
#include <netdb.h>
#include <spdlog/spdlog.h>

chs::OutgoingMessageQueue::OutgoingMessageQueue(const chs::WebSocket & socket) : socket(socket), sending(false), sentOffset(0){
}

void chs::OutgoingMessageQueue::putMessage(chs::Message message) {
    queue.push(std::move(message));
}

void chs::OutgoingMessageQueue::sendMessages() {
    while (not queue.empty() and not blocked) {
        if (not sending) {
            currentMessage = chs::constructMessage(queue.front(), queue.front().size());
            queue.pop();
            sending = true;
            sentOffset = 0;
        }

        if (sending) {
            auto startingAddress = currentMessage.data() + sentOffset;
            auto remainingLength = currentMessage.size() - sentOffset;
            auto sentBytes = send(socket.getDescriptor(),  startingAddress, remainingLength, 0);
            sentOffset += sentBytes;
            if (sentOffset == currentMessage.size()) {
                sending = false;
            } else {
                blocked = true;
            }
        }
    }
}

bool chs::OutgoingMessageQueue::isBlocked() {
    return blocked;
}
