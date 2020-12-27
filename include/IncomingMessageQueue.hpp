//
// Created by hubert on 27.12.2020.
//

#pragma once

#include <queue>
#include "IncomingMessage.hpp"

namespace chs{
    class IncomingMessageQueue {
    private:
        std::queue<chs::Message> queue;
        int messageSize;
        int socket;
        bool reading;

    public:
        explicit IncomingMessageQueue(int socket);
        void readMessages();
        chs::Message getMessage();

    private:
        [[nodiscard]] int getBufferSize() const;
    };

}



