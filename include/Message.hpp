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

    char * serializeMessage(char * message, const char* arg) {
        strcpy(message, arg);
        return message + strlen(arg) + 1;
    }

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
    std::string deserializeMessage<std::string>(char *& message) {
        std::string object{message};
        message += object.size() + 1;
        return object;
    }

    template<typename ... T>
    std::tuple<T...> deconstructMessage(chs::Message & message) {
        auto ptr = message.data();
        return std::make_tuple(deserializeMessage<T>(ptr)...);
    }

    MessageType getMessageType(const chs::Message & message) {
        MessageType type;
        auto addr = message.data() + message.size() - sizeof(type);
        memcpy(&type, addr, sizeof(type));
        return type;
    }
}

#endif //MESSAGE_HPP
