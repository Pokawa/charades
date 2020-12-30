//
// Created by hubert on 27.12.2020.
//

#pragma once

#include <string>
#include <cstring>


namespace chs{
    using Message = std::string;

    std::size_t messageSize(const std::string& arg) {
        return arg.size() + 1;
    }

    template<typename T>
    std::size_t messageSize(const T& arg) {
        return sizeof(arg);
    }

    template<typename T, typename ... L>
    std::size_t messageSize(const T& arg, const L& ...leftOver) {
        return messageSize(arg) + messageSize(leftOver...);
    }

    char * serializeMessage(char * message, const std::string& arg) {
        strcpy(message, arg.data());
        return message + arg.size() + 1;
    }

    template<typename T>
    char * serializeMessage(char * message, const T& arg) {
        memcpy(message, &arg, sizeof(arg));
        return message + sizeof(arg);
    }

    template<typename T, typename ... L>
    char * serializeMessage(char * message, const T& arg, const L& ... args){
        auto addr = serializeMessage(message, arg);
        return serializeMessage(addr, args...);
    }

    template<typename ... T>
    chs::Message constructMessage(const T& ... args){
        auto size = messageSize(args...);
        chs::Message message(size, 0);
        serializeMessage(message.data(), args...);
        return message;
    }
}

