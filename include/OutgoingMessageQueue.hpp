//
// Created by hubert on 29.12.2020.
//

#ifndef OUTGOINGMESSAGEQUEUE_HPP
#define OUTGOINGMESSAGEQUEUE_HPP

#include <queue>
#include "WebSocket.hpp"

namespace chs {
    using Message = std::string;

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

#endif //OUTGOINGMESSAGEQUEUE_HPP

