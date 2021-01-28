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
        auto ret = recv(socketFD, message.data(), messageSize, MSG_WAITALL);

        if (ret == 0) {
            spdlog::critical("Server hanged up");
            emit serverHangUp();
            break;
        }

        if (ret == -1) {
            spdlog::critical("Message receive error {}", strerror(errno));
            emit errorOccurred();
            break;
        }

        //emit signal
        emit messageReceived(message);
    }
}

MessageReceiver::MessageReceiver(int socketFD) : socketFD(socketFD) {
}
