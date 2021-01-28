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
        LOG_IN_REQUEST,
        LOG_OUT_REQUEST,
        ROOMS_INFO_REQUEST,
        NEW_ROOM_REQUEST,
        JOIN_ROOM_REQUEST,
        QUIT_ROOM_REQUEST,
        START_GAME_REQUEST,
        ENTER_DRAWING_QUEUE_REQUEST,
        QUIT_DRAWING_QUEUE_REQUEST,
        CHAT_MESSAGE,
        DRAW_LINE,
        CLEAR_DRAWING,
        OK_RESPOND,
        ERROR_RESPOND,
        ROOM_INFO_RESPOND,
        IN_GAME_INFO_RESPOND,
        CHARADES_WORD_RESPOND,
        SERVER_MESSAGE,
    };

    [[nodiscard]] MessageType getMessageType(const chs::Message & message);

    [[nodiscard]] std::string joinStrings(const std::vector<std::string>& container, char delimiter);

    [[nodiscard]] std::vector<std::string> explodeJoinedString(const std::string & joined, char delimiter);

    [[nodiscard]] std::string utf8_substr(const std::string& str, std::string::size_type start, std::string::size_type leng);

    template<typename T>
    [[nodiscard]] std::size_t messageSize(const T& arg) {
        return sizeof(arg);
    }

    template<>
    [[nodiscard]] std::size_t messageSize(const std::string& arg);

    template<typename T, typename ... L>
    [[nodiscard]] std::size_t messageSize(const T& arg, const L& ...leftOver) {
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
    [[nodiscard]] chs::Message constructMessage(const T& ... args){
        auto size = messageSize(args...);
        chs::Message message(size + sizeof(size), 0);
        serializeMessage(message.data(), args..., size);
        return message;
    }

    template<typename T>
    [[nodiscard]] T deserializeMessage(char *& message) {
        T object;
        memcpy(&object, message, sizeof(T));
        message += sizeof(T);
        return object;
    }

    template<>
    [[nodiscard]] std::string deserializeMessage<std::string>(char *& message);

    template<typename ... T>
    [[nodiscard]] std::tuple<T...> deconstructMessage(chs::Message & message) {
        auto ptr = message.data();
        return std::make_tuple(deserializeMessage<T>(ptr)...);
    }

    [[nodiscard]] chs::Message addSizeToMessage(const chs::Message & message);
}

#endif //MESSAGE_HPP
