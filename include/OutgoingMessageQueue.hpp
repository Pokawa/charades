//
// Created by hubert on 29.12.2020.
//

#pragma once

#include <queue>
#include "Message.hpp"
#include "WebSocket.hpp"

namespace chs {
    class OutgoingMessageQueue {
    private:
        std::queue<chs::Message> queue;
        const chs::WebSocket & socket;
        chs::Message currentMessage;
        bool sending;
        int sentOffset;

    public:
        explicit OutgoingMessageQueue(const chs::WebSocket & socket);
        void sendMessages();
        void putMessage(chs::Message);
    };
}


