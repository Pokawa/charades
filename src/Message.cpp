//
// Created by hubert on 30.12.2020.
//

#include <Message.hpp>
#include <sstream>

std::string chs::joinStrings(const std::vector<std::string>& container, char delimiter) {
    if (container.empty()) {
        return "";
    }

    std::string del{delimiter};

    std::ostringstream os;
    std::ostream_iterator<std::string> iter (os, del.data());
    std::copy(container.begin(), container.end() - 1, iter);
    os << *container.rbegin();
    return os.str();
}

std::vector<std::string> chs::explodeJoinedString(const std::string &joined, char delimiter)
{
    std::vector<std::string> result;
    std::istringstream iss(joined);

    for (std::string token; std::getline(iss, token, delimiter); )
    {
        result.push_back(std::move(token));
    }

    return result;
}

std::string chs::utf8_substr(const std::string& str, std::string::size_type start, std::string::size_type leng)
{
    if (leng==0) { return ""; }
    unsigned int c, i, ix, q;
    auto min=std::string::npos, max=std::string::npos;
    for (q=0, i=0, ix=str.length(); i < ix; i++, q++)
    {
        if (q==start){ min=i; }
        if (q<=start+leng || leng == std::string::npos){ max=i; }

        c = (unsigned char) str[i];
        if      (c<=127) i+=0;
        else if ((c & 0xE0) == 0xC0) i+=1;
        else if ((c & 0xF0) == 0xE0) i+=2;
        else if ((c & 0xF8) == 0xF0) i+=3;
        else return "";
    }
    if (q<=start+leng || leng==std::string::npos){ max=i; }
    if (min==std::string::npos || max==std::string::npos) { return ""; }
    return str.substr(min,max);
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

chs::Message chs::addSizeToMessage(const chs::Message & message) {
    auto messageSize = message.size();
    chs::Message newMessage( messageSize + sizeof(messageSize), 0);
    memcpy(newMessage.data(), &messageSize, sizeof(messageSize));
    memcpy(newMessage.data() + sizeof(messageSize), message.data(), messageSize);
    return newMessage;
}