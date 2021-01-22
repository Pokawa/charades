//
// Created by hubert on 22.01.2021.
//

#include <poll.h>
#include <csignal>
#include <resolv.h>
#include "MessageReceiver.hpp"
#include "ServerHandler.hpp"

MessageReceiver MessageReceiver::instance;

MessageReceiver &MessageReceiver::getInstance() {
    return instance;
}

void MessageReceiver::run() {
    auto socketFD = ServerHandler::getInstance().getSocketFD();
    pollfd pollSocket = {.fd = socketFD, .events = POLL_IN, .revents = 0};
    while (true) {
        auto ret = poll(&pollSocket, 1, -1);
        if (ret == 1) {
            if (pollSocket.revents & POLL_IN) {
                //Take message size
                std::size_t messageSize{0};
                recv(socketFD, &messageSize, sizeof(messageSize), MSG_WAITALL);

                //read message
                chs::Message message(messageSize, 0);
                recv(socketFD, message.data(), messageSize, MSG_WAITALL);

                //emit signal
                emit messageReceived(message);
            }
        }
    }
}
