//
// Created by hubert on 30.12.2020.
//

#include <Message.hpp>
#include <sstream>

std::string chs::joinStrings(const std::vector<std::string>& container, const std::string & delimiter) {
    if (container.empty()) {
        return "";
    }

    std::ostringstream os;
    std::ostream_iterator<std::string> iter (os, delimiter.data());
    std::copy(container.begin(), container.end() - 1, iter);
    os << *container.rbegin();
    return os.str();
}

template<>
std::size_t chs::messageSize(const std::string& arg) {
    return arg.size() + 1;
}

char * chs::serializeMessage(char * message, const std::string& arg) {
    memcpy(message, arg.data(), arg.size());
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