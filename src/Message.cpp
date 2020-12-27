//
// Created by hubert on 27.12.2020.
//

#include "Message.hpp"


chs::Message::Message(uint expectedSize) : std::string(expectedSize, 0), expectedSize(expectedSize) {
}

bool chs::Message::isComplete() {
    return size() == expectedSize;
}

uint chs::Message::getRemainingSize() {
    return size() - expectedSize;
}
