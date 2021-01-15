//
// Created by hubert on 27.12.2020.
//

#include <resolv.h>
#include <sys/ioctl.h>
#include "IncomingMessageQueue.hpp"

chs::IncomingMessageQueue::IncomingMessageQueue(const chs::Socket & socket) : socket(socket), reading(false), messageSize(0) {
}

void chs::IncomingMessageQueue::readMessages() {
    while (getBufferSize() >= sizeof(std::size_t)) {
        if (not reading) {
            reading = true;
            recv(socket.getDescriptor(), &messageSize, sizeof(messageSize), 0);
        } else if (getBufferSize() >= messageSize) {
            chs::Message message(messageSize, 0);
            recv(socket.getDescriptor(), message.data(), messageSize, 0);
            //TODO error handling
            queue.push(std::move(message));
            reading = false;
        }
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
