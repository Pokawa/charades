//
// Created by hubert on 27.12.2020.
//

#ifndef INGOINGMESSAGEQUEUE_HPP
#define INGOINGMESSAGEQUEUE_HPP


#include <queue>
#include "Socket.hpp"

namespace chs{
    using Message = std::string;

    class IncomingMessageQueue {
    private:
        std::queue<chs::Message> queue;
        std::size_t messageSize;
        std::size_t readBytes;
        std::string currentMessage;
        const chs::Socket socket;
        bool reading;

    public:
        explicit IncomingMessageQueue(const chs::Socket & socket);
        bool readMessages();
        chs::Message getMessage();
        bool isNotEmpty();

    private:
        [[nodiscard]] int getBufferSize() const;
    };

}

#endif //INGOINGMESSAGEQUEUE_HPP


