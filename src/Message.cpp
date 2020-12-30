//
// Created by hubert on 30.12.2020.
//

#include <Message.hpp>

std::size_t chs::messageSize(const std::string& arg) {
    return arg.size() + 1;
}

char * chs::serializeMessage(char * message, const std::string& arg) {
    strcpy(message, arg.data());
    return message + arg.size() + 1;
}

char * chs::serializeMessage(char * message, const char* arg) {
    strcpy(message, arg);
    return message + strlen(arg) + 1;
}

template<>
std::string chs::deserializeMessage<std::string>(char *& message) {
    std::string object{message};
    message += object.size() + 1;
    return object;
}

chs::MessageType chs::getMessageType(const chs::Message & message) {
    MessageType type;
    auto addr = message.data() + message.size() - sizeof(type);
    memcpy(&type, addr, sizeof(type));
    return type;
}