//
// Created by hubert on 22.01.2021.
//

#include <netdb.h>
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <QPoint>
#include <QColor>
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
            if (lastRequestToConfirm == chs::MessageType::LOG_IN_REQUEST) {
                emit loginSuccessful();
            }
            break;
        case chs::MessageType::ERROR_RESPOND:
            if (lastRequestToConfirm == chs::MessageType::LOG_IN_REQUEST) {
                emit loginFailed();
            }
            break;
        case chs::MessageType::ROOM_INFO_RESPOND: {
            emit roomsInfoRespond(message);
        }
            break;
        case chs::MessageType::IN_GAME_INFO_RESPOND: {
            if (lastRequestToConfirm == chs::MessageType::NEW_ROOM_REQUEST or lastRequestToConfirm == chs::MessageType::JOIN_ROOM_REQUEST) {
                emit joinedRoom();
            }
            emit inGameInfoRespond(message);
        }
            break;
        case chs::MessageType::CLEAR_DRAWING:
            emit drawingCleared();
            break;
        case chs::MessageType::DRAW_LINE: {
            auto [x1, y1, x2, y2, rgb] = chs::deconstructMessage<int, int, int, int, QRgb>(message);
            emit linePainted(QPoint(x1, y1), QPoint(x2, y2), QColor(rgb));
        }
            break;
        case chs::MessageType::CHAT_MESSAGE: {
            auto [chat] = chs::deconstructMessage<std::string>(message);
            emit receivedChatMessage(chat);
        }
            break;
        case chs::MessageType::SERVER_MESSAGE: {
            auto [chat] = chs::deconstructMessage<std::string>(message);
            emit receivedServerMessage(chat);
        }
            break;
        default:
            spdlog::info("Unknown message received. type: {} message: {]", type, message);
            break;
    }
}

void CommunicationHandler::logInRequest(const std::string& username) {
    auto loginMessage = chs::constructMessage(chs::MessageType::LOG_IN_REQUEST, username);
    lastRequestToConfirm = chs::MessageType::LOG_IN_REQUEST;
    sendMessage(loginMessage);
}

void CommunicationHandler::sendMessage(const chs::Message &message) {
    auto ret = send(socketFD, message.data(), message.size(), 0);
}

CommunicationHandler::~CommunicationHandler() {
    disconnectFromHost();
}

void CommunicationHandler::disconnectFromHost() {
    stopMessageReceiver();
    sendRequest(chs::MessageType::LOG_OUT_REQUEST);
    close(socketFD);
}

void CommunicationHandler::stopMessageReceiver() {
    asyncMessageReceiver.terminate();
    asyncMessageReceiver.wait();
}

void CommunicationHandler::roomsInfoRequest() {
    sendRequest(chs::MessageType::ROOMS_INFO_REQUEST);
}

void CommunicationHandler::joinRoomRequest(int roomNumber) {
    auto joinRoomRequest = chs::constructMessage(chs::MessageType::JOIN_ROOM_REQUEST, roomNumber);
    lastRequestToConfirm = chs::MessageType::JOIN_ROOM_REQUEST;
    sendMessage(joinRoomRequest);
}

void CommunicationHandler::newRoomRequest() {
    sendRequest(chs::MessageType::NEW_ROOM_REQUEST);
    lastRequestToConfirm = chs::MessageType::NEW_ROOM_REQUEST;
}

void CommunicationHandler::exitRoomRequest() {
    sendRequest(chs::MessageType::QUIT_ROOM_REQUEST);
}

void CommunicationHandler::sendRequest(chs::MessageType request) {
    auto requestMessage = chs::constructMessage(request);
    sendMessage(requestMessage);
}

void CommunicationHandler::startGameRequest() {
    sendRequest(chs::MessageType::START_GAME_REQUEST);
}

void CommunicationHandler::enterDrawingQueueRequest() {
    sendRequest(chs::MessageType::ENTER_DRAWING_QUEUE_REQUEST);
}

void CommunicationHandler::leaveDrawingQueueRequest() {
    sendRequest(chs::MessageType::QUIT_DRAWING_QUEUE_REQUEST);
}

void CommunicationHandler::sendChatMessageRequest(const std::string& message) {
    auto chatMessage = chs::constructMessage(chs::MessageType::CHAT_MESSAGE, message);
    sendMessage(chatMessage);
}

void CommunicationHandler::drawLineRequest(QPoint pt1, QPoint pt2, const QColor& color) {
    auto drawLineMessage = chs::constructMessage(chs::MessageType::DRAW_LINE, pt1.x(), pt1.y(), pt2.x(), pt2.y(), color.rgb());
    sendMessage(drawLineMessage);
}

void CommunicationHandler::clearRequest() {
    sendRequest(chs::MessageType::CLEAR_DRAWING);
}
