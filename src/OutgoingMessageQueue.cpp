//
// Created by hubert on 29.12.2020.
//

#include "OutgoingMessageQueue.hpp"
#include <netdb.h>
#include <spdlog/spdlog.h>

chs::OutgoingMessageQueue::OutgoingMessageQueue(const chs::Socket & socket) : socket(socket), sending(false), sentOffset(0), blocked(false){
}

void chs::OutgoingMessageQueue::putMessage(chs::Message message) {
    queue.push(std::move(message));
}

bool chs::OutgoingMessageQueue::sendMessages() {
    blocked = false;

    while (not queue.empty() and not blocked) {
        if (not sending) {
            currentMessage = queue.front();
            queue.pop();
            sending = true;
            sentOffset = 0;
        }

        if (sending) {
            auto startingAddress = currentMessage.data() + sentOffset;
            auto remainingLength = currentMessage.size() - sentOffset;
            auto sentBytes = send(socket.getDescriptor(),  startingAddress, remainingLength, MSG_DONTWAIT);

            if (sentBytes == -1) {
                spdlog::error("Sending to socket error: {}", strerror(errno));
                return false;
            }

            sentOffset += sentBytes;
            if (sentOffset == currentMessage.size()) {
                sending = false;
            } else {
                blocked = true;
            }
        }
    }
    return true;
}

bool chs::OutgoingMessageQueue::isBlocked() const {
    return blocked;
}

bool chs::OutgoingMessageQueue::putAndSendMessage(chs::Message message) {
    putMessage(std::move(message));
    return sendMessages();
}
