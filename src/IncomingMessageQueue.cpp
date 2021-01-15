//
// Created by hubert on 27.12.2020.
//

#include <resolv.h>
#include <sys/ioctl.h>
#include "IncomingMessageQueue.hpp"

chs::IncomingMessageQueue::IncomingMessageQueue(const chs::Socket & socket) : socket(socket), reading(false), messageSize(0), readBytes(0) {
}

void chs::IncomingMessageQueue::readMessages() {
    while ((getBufferSize() > 0 and reading) or (not reading and getBufferSize() >= sizeof(std::size_t))){

        if (not reading) {
            reading = true;
            recv(socket.getDescriptor(), &messageSize, sizeof(messageSize), 0);
            currentMessage = std::string(messageSize, 0);
            readBytes = 0;
        }

        auto writeOffset = currentMessage.data() + readBytes;
        auto remainingSize = messageSize - readBytes;
        auto bytes = recv(socket.getDescriptor(), writeOffset, remainingSize, 0);

        if (bytes == -1) {
            //TODO error handling
        } else {
            readBytes += bytes;
        }

        if (readBytes == messageSize) {
            queue.push(std::move(currentMessage));
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
