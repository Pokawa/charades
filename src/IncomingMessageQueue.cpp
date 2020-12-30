//
// Created by hubert on 27.12.2020.
//

#include <resolv.h>
#include <sys/ioctl.h>
#include "IncomingMessageQueue.hpp"

chs::IncomingMessageQueue::IncomingMessageQueue(const chs::WebSocket & socket) : socket(socket), reading(false), messageSize(0) {
}

void chs::IncomingMessageQueue::readMessages() {
    if (getBufferSize() >= sizeof(std::size_t) and not reading) {
        reading = true;
        recv(socket.getDescriptor(), &messageSize, sizeof(messageSize), 0);
    }

    if (getBufferSize() >= messageSize and reading) {
        chs::Message message(messageSize, 0);
        auto received = recv(socket.getDescriptor(), message.data(), messageSize, 0);
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
    ioctl(socket.getDescriptor(),FIONREAD,&bytes_available);
    return bytes_available;
}

bool chs::IncomingMessageQueue::isNotEmpty() {
    return !queue.empty();
}
