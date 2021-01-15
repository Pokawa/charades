//
// Created by hubert on 27.12.2020.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <cstring>
#include <spdlog/spdlog.h>

namespace chs{
    using Message = std::string;

    enum class MessageType {
        LOG_IN,
        LOG_OUT
    };

    MessageType getMessageType(const chs::Message & message);



    template<typename T>
    std::size_t messageSize(const T& arg) {
        return sizeof(arg);
    }

    template<>
    std::size_t messageSize(const std::string& arg);

    template<typename T, typename ... L>
    std::size_t messageSize(const T& arg, const L& ...leftOver) {
        return messageSize(arg) + messageSize(leftOver...);
    }

    char * serializeMessage(char * message, const std::string& arg);
    char * serializeMessage(char * message, const char* arg);

    template<typename T>
    char * serializeMessage(char * message, const T& arg) {
        memcpy(message, &arg, sizeof(arg));
        return message + sizeof(arg);
    }

    template<typename T, typename ... L>
    char * serializeMessage(char * message, const T& arg, const L& ... args){
        auto addr = serializeMessage(message, args...);
        return serializeMessage(addr, arg);
    }

    template<typename ... T>
    chs::Message constructMessage(const T& ... args){
        auto size = messageSize(args...);
        chs::Message message(size, 0);
        serializeMessage(message.data(), args...);
        return message;
    }

    template<typename T>
    T deserializeMessage(char *& message) {
        T object;
        memcpy(&object, message, sizeof(T));
        message += sizeof(T);
        return object;
    }

    template<>
    std::string deserializeMessage<std::string>(char *& message);

    template<typename ... T>
    std::tuple<T...> deconstructMessage(chs::Message & message) {
        auto ptr = message.data();
        return std::make_tuple(deserializeMessage<T>(ptr)...);
    }
}

#endif //MESSAGE_HPP
