//
// Created by hubert on 27.12.2020.
//

#include <resolv.h>
#include <sys/ioctl.h>
#include "IncomingMessageQueue.hpp"

chs::IncomingMessageQueue::IncomingMessageQueue(int socket) : socket(socket), reading(false), messageSize(0) {
}

void chs::IncomingMessageQueue::readMessages() {
    if (getBufferSize() >= 4 and not reading) {
        reading = true;
        recv(socket, &messageSize, sizeof(messageSize), 0);
    }

    if (getBufferSize() >= messageSize and reading) {
        chs::IncomingMessage message(messageSize, 0);
        recv(socket, message.data(), messageSize, 0);
        queue.push(std::move(message));
        reading = false;
    }
}

chs::Message chs::IncomingMessageQueue::getMessage() {
    auto message = std::move(queue.front());
    queue.pop();
    return std::move(message);
}

int chs::IncomingMessageQueue::getBufferSize() const {
    int bytes_available{0};
    ioctl(socket,FIONREAD,&bytes_available);
    return bytes_available;
}
