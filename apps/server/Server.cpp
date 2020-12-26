//
// Created by hubert on 26.12.2020.
//

#include <netdb.h>
#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <arpa/inet.h>

#include <utility>
#include "Server.hpp"

Server::Server(std::string port) : port(std::move(port)){
}

bool Server::acceptClient() {
    sockaddr_in clientAddr{0};
    socklen_t clientAddrSize = sizeof(clientAddr);

    auto clientFd = accept(this->serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);
    if (clientFd == -1) {
        spdlog::error("Client acceptance failed");
        return false;
    } else {
        spdlog::info("New client connected from {} on {}", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        this->clientsSockets.insert(clientFd);
        return true;
    }
}

void Server::openServer() {
    addrinfo hints { .ai_flags = AI_PASSIVE, .ai_protocol = IPPROTO_TCP};
    addrinfo *resolved;

    if (getaddrinfo(nullptr, port.c_str(), &hints, &resolved)) {
        throw ServerExeption(fmt::format("Resolving address failed: {}", strerror(errno)));
    }

    this->serverSocket = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    const int one = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))) {
        throw ServerExeption(fmt::format("Failed setting socket to address reuse: {}", strerror(errno)));
    }

    if (bind(this->serverSocket, resolved->ai_addr, resolved->ai_addrlen)) {
        throw ServerExeption(fmt::format("Failed to bind: {}", strerror(errno)));
    }

    freeaddrinfo(resolved);
    listen(this->serverSocket, 10);

    spdlog::info("Successfully started server on port {}", this->port);
}
