//
// Created by hubert on 29.12.2020.
//

#include "OutgoingMessageQueue.hpp"
#include <sys/ioctl.h>
#include <netdb.h>

chs::OutgoingMessageQueue::OutgoingMessageQueue(int socket) : socket(socket), sending(false), sentOffset(0){
}

void chs::OutgoingMessageQueue::putMessage(chs::Message message) {
    queue.push(std::move(message));
}

void chs::OutgoingMessageQueue::sendMessages() {
    if (not sending and not queue.empty()) {
        currentMessage = std::move(queue.front());
        queue.pop();
        sending = true;
        sentOffset = 0;

        auto messageSize = currentMessage.size();
        send(socket, &messageSize, sizeof(messageSize), 0);
        //TODO error handling
    }

    if (sending) {
        auto startingAddress = currentMessage.data() + sentOffset;
        auto remainingLength = currentMessage.size() - sentOffset;
        auto sentBytes = send(socket,  startingAddress, remainingLength, 0);
        //TODO error handling
        sentOffset += sentBytes;

        if (sentOffset == currentMessage.size())
            sending = false;
    }
}