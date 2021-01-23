//
// Created by hubert on 22.01.2021.
//

#include <csignal>
#include <resolv.h>
#include "MessageReceiver.hpp"

void MessageReceiver::run() {
    while (true) {
        std::size_t messageSize{0};
        recv(socketFD, &messageSize, sizeof(messageSize), MSG_WAITALL);

        //read message
        chs::Message message(messageSize, 0);
        recv(socketFD, message.data(), messageSize, MSG_WAITALL);

        //emit signal
        emit messageReceived(message);
    }
}

MessageReceiver::MessageReceiver(int socketFD) : socketFD(socketFD) {
}
