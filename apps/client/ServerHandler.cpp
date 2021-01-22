//
// Created by hubert on 22.01.2021.
//

#include <netdb.h>
#include <spdlog/spdlog.h>
#include "ServerHandler.hpp"
#include "MessageReceiver.hpp"

ServerHandler ServerHandler::instance;

ServerHandler& ServerHandler::getInstance() {
    return instance;
}

bool ServerHandler::connectToHost(const std::string &host, int port) {
    // Resolve arguments to IPv4 address with a port number
    addrinfo *resolved, hints = {.ai_flags = 0, .ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
    int res = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &resolved);
    if (res || !resolved) {
        spdlog::error("Host name resolving error: {}", gai_strerror(res));
        return false;
    }

    // create socket
    int sock = socket(resolved->ai_family, resolved->ai_socktype, 0);
    if (sock == -1) {
        spdlog::error("Creating socket error: {}", strerror(errno));
        return false;
    }

    // attempt to connectToHost
    res = ::connect(sock, resolved->ai_addr, resolved->ai_addrlen);
    if (res) {
        spdlog::error("Connecting failed: {}", strerror(errno));
        return false;
    }

    socketFD = sock;
    freeaddrinfo(resolved);

    connect(&MessageReceiver::getInstance(), &MessageReceiver::messageReceived, this, &ServerHandler::handleMessage);
    MessageReceiver::getInstance().start();
    return true;
}

int ServerHandler::getSocketFD() {
    return socketFD;
}

ServerHandler::ServerHandler() : socketFD(0) {
}

void ServerHandler::handleMessage(const chs::Message& message) {
    spdlog::info("received message type:{}", chs::getMessageType(message));
}

