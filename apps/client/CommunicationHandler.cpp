//
// Created by hubert on 22.01.2021.
//

#include <netdb.h>
#include <spdlog/spdlog.h>
#include <unistd.h>
#include "CommunicationHandler.hpp"
#include "MessageReceiver.hpp"

int CommunicationHandler::connectToHost(const std::string &host, int port) {
    // Resolve arguments to IPv4 address with a port number
    addrinfo *resolved, hints = {.ai_flags = 0, .ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
    int res = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &resolved);
    if (res || !resolved) {
        spdlog::error("Host name resolving error: {}", gai_strerror(res));
        return -1;
    }

    // create socket
    int sock = socket(resolved->ai_family, resolved->ai_socktype, 0);
    if (sock == -1) {
        spdlog::error("Creating socket error: {}", strerror(errno));
        return -1;
    }

    // attempt to connectToHost
    res = ::connect(sock, resolved->ai_addr, resolved->ai_addrlen);
    if (res) {
        spdlog::error("Connecting failed: {}", strerror(errno));
        return -1;
    }

    freeaddrinfo(resolved);
    return sock;
}

int CommunicationHandler::getSocketFD() const {
    return socketFD;
}

CommunicationHandler::CommunicationHandler(int socketFD) : socketFD(socketFD), asyncMessageReceiver(socketFD) {
    connect(&asyncMessageReceiver, &MessageReceiver::messageReceived, this, &CommunicationHandler::handleMessage);
    asyncMessageReceiver.start();
}

void CommunicationHandler::handleMessage(chs::Message message) {
    auto type = chs::getMessageType(message);
    switch (type) {
        case chs::MessageType::OK_RESPOND:
            if (lastRequest == chs::MessageType::LOG_IN_REQUEST) {
                emit loginSuccessful();
            }
            break;
        case chs::MessageType::ERROR_RESPOND:
            if (lastRequest == chs::MessageType::LOG_IN_REQUEST) {
                emit loginFailed();
            }
            break;
        case chs::MessageType::ROOM_INFO_RESPOND: {
            auto[roomNumber, joinedNames] = chs::deconstructMessage<int, std::string>(message);
            emit roomsInfoRespond(roomNumber, joinedNames);
        }
            break;
        default:
            spdlog::info("Unknown message received. type: {} message: {]", type, message);
            break;
    }
}

void CommunicationHandler::logInRequest(const std::string& username) {
    auto loginMessage = chs::constructMessage(chs::MessageType::LOG_IN_REQUEST, username);
    lastRequest = chs::MessageType::LOG_IN_REQUEST;
    sendMessage(loginMessage);
}

void CommunicationHandler::sendMessage(const chs::Message &message) {
    send(socketFD, message.data(), message.size(), 0);
}

CommunicationHandler::~CommunicationHandler() {
    disconnectFromHost();
}

void CommunicationHandler::disconnectFromHost() {
    stopMessageReceiver();
    auto disconnectMessage = chs::constructMessage(chs::MessageType::LOG_OUT_REQUEST);
    sendMessage(disconnectMessage);
    close(socketFD);
}

void CommunicationHandler::stopMessageReceiver() {
    asyncMessageReceiver.terminate();
    asyncMessageReceiver.wait();
}

void CommunicationHandler::roomsInfoRequest() {
    auto roomsInfoMessage = chs::constructMessage(chs::MessageType::ROOMS_INFO_REQUEST);
    lastRequest = chs::MessageType::ROOMS_INFO_REQUEST;
    sendMessage(roomsInfoMessage);
}

void CommunicationHandler::joinRoomRequest(int roomNumber) {
    auto joinRoomRequest = chs::constructMessage(chs::MessageType::JOIN_ROOM_REQUEST, roomNumber);
    lastRequest = chs::MessageType::JOIN_ROOM_REQUEST;
    sendMessage(joinRoomRequest);
}

void CommunicationHandler::newRoomRequest() {
    auto newRoomRequest = chs::constructMessage(chs::MessageType::NEW_ROOM_REQUEST);
    lastRequest = chs::MessageType::NEW_ROOM_REQUEST;
    sendMessage(newRoomRequest);
}
