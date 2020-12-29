//
// Created by hubert on 27.12.2020.
//

#pragma once

#include <queue>
#include "IncomingMessage.hpp"
#include "WebSocket.hpp"

namespace chs{
    class IncomingMessageQueue {
    private:
        std::queue<chs::Message> queue;
        int messageSize;
        const chs::WebSocket & socket;
        bool reading;

    public:
        explicit IncomingMessageQueue(const chs::WebSocket & socket);
        void readMessages();
        chs::Message getMessage();

    private:
        [[nodiscard]] int getBufferSize() const;
    };

}



