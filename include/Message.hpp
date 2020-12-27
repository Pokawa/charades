//
// Created by hubert on 27.12.2020.
//

#pragma once

#include <string>

namespace chs{
    class Message : std::string {
    private:
        uint expectedSize;

    public:
        explicit Message(uint expectedSize);
        bool isComplete();
        uint getRemainingSize();
    };
}

