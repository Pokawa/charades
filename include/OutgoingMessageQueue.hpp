//
// Created by hubert on 29.12.2020.
//

#ifndef OUTGOINGMESSAGEQUEUE_HPP
#define OUTGOINGMESSAGEQUEUE_HPP

#include <queue>
#include "Socket.hpp"
#include "Message.hpp"

namespace chs {
    using Message = std::string;

    class OutgoingMessageQueue {
    private:
        std::queue<chs::Message> queue;
        const chs::Socket & socket;
        chs::Message currentMessage;
        bool sending;
        bool blocked;
        int sentOffset;

    public:
        explicit OutgoingMessageQueue(const chs::Socket & socket);
        bool sendMessages();
        void putMessage(chs::Message);
        bool putAndSendMessage(chs::Message);
        [[nodiscard]] bool isBlocked() const;
    };
}

#endif //OUTGOINGMESSAGEQUEUE_HPP

