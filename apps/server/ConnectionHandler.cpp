//
// Created by hubert on 26.12.2020.
//

#include <netdb.h>
#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <arpa/inet.h>
#include <utility>
#include "ConnectionHandler.hpp"

ConnectionHandler::ConnectionHandler(std::string port) : port(std::move(port)), serverSocket(0){
    openServer();
}

bool ConnectionHandler::acceptClient() {
    sockaddr_in clientAddr{0};
    socklen_t clientAddrSize = sizeof(clientAddr);
    auto clientFd = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);

    if (clientFd == -1) {
        spdlog::error("Client acceptance failed");
        return false;
    } else {
        clientsSockets.emplace_back(clientFd, clientAddr);
        spdlog::info("New client connected from {} on {}", clientsSockets.back().getAddress(), clientsSockets.back().getPort());
        return true;
    }
}

void ConnectionHandler::openServer() {
    addrinfo hints { .ai_flags = AI_PASSIVE, .ai_protocol = IPPROTO_TCP};
    addrinfo *resolved;

    if (getaddrinfo(nullptr, port.c_str(), &hints, &resolved)) {
        spdlog::critical("Resolving address failed: {}", strerror(errno));
        exit(1);
    }

    serverSocket = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    const int one = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))) {
        spdlog::critical("Failed setting socket to address reuse: {}", strerror(errno));
        exit(2);
    }

    if (bind(serverSocket, resolved->ai_addr, resolved->ai_addrlen)) {
        spdlog::critical("Failed to bind: {}", strerror(errno));
        exit(3);
    }

    freeaddrinfo(resolved);
    listen(this->serverSocket, 10);

    spdlog::info("Successfully started server on port {}", this->port);
}
