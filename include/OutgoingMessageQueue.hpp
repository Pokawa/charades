//
// Created by hubert on 29.12.2020.
//

#pragma once

#include <queue>
#include "IncomingMessage.hpp"

namespace chs {
    class OutgoingMessageQueue {
    private:
        std::queue<chs::Message> queue;
        int socket;
        chs::Message currentMessage;
        bool sending;
        int sentOffset;

    public:
        explicit OutgoingMessageQueue(int socket);
        void sendMessages();
        void putMessage(chs::Message);


    };
}


