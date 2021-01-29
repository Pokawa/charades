//
// Created by hubert on 26.12.2020.
//

#include <netdb.h>
#include <spdlog/spdlog.h>
#include <arpa/inet.h>
#include <utility>
#include <fcntl.h>
#include "ConnectionHandler.hpp"

ConnectionHandler::ConnectionHandler(std::string port) : port(std::move(port)), serverSocket(0){
}

bool ConnectionHandler::acceptClient() {
    sockaddr_in clientAddr{0};
    socklen_t clientAddrSize = sizeof(clientAddr);
    auto clientFd = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);

    if (clientFd == -1) {
        spdlog::error("Client acceptance failed {}", strerror(errno));
        return false;
    } else {
        setSocketToNonBlock(clientFd);
        clientsSockets.emplace_back(clientFd, clientAddr);
        addToPoll(clientsSockets.back().getDescriptor(), POLLIN);

        spdlog::info("New client connected from {} on {}", clientsSockets.back().getAddress(), clientsSockets.back().getPort());
        return true;
    }
}

void ConnectionHandler::openServer() {
    addrinfo hints {0};
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_TCP;
    addrinfo *resolved;

    if (getaddrinfo(nullptr, port.c_str(), &hints, &resolved)) {
        spdlog::critical("Resolving address failed: {}", strerror(errno));
        exit(1);
    }

    serverSocket = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    setSocketToNonBlock(serverSocket);

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
    if (listen(serverSocket, 10)) {
        spdlog::critical("Failed to set listen on: {}", strerror(errno));
        exit(4);
    }

    addToPoll(serverSocket, POLLIN);

    spdlog::info("Successfully started server on port {}", this->port);
}

std::vector<pollfd>ConnectionHandler::getPollSockets() {
    return pollSockets;
}

void ConnectionHandler::setSocketToNonBlock(int socket) {
    auto flags = fcntl(socket, F_GETFL);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void ConnectionHandler::addToPoll(int socket, short events) {
    pollSockets.push_back({.fd = socket, .events = events});
}

void ConnectionHandler::removeFromPoll(int socket) {
    auto pos = std::find_if(pollSockets.begin(), pollSockets.end(),
                            [&socket](const pollfd & sock){ return sock.fd == socket; });
    if (pos != pollSockets.end())
        pollSockets.erase(pos);
}

const chs::Socket& ConnectionHandler::getLastClient() {
    return clientsSockets.back();
}

const chs::Socket &ConnectionHandler::getSocket(const int &socket) {
    auto position = std::find_if(clientsSockets.begin(), clientsSockets.end(),
                                 [&socket](const chs::Socket & sock){ return sock.getDescriptor() == socket; });
    return *position;
}

void ConnectionHandler::closeClient(chs::Socket & client) {
    auto pos = std::find_if(clientsSockets.begin(), clientsSockets.end(),
                            [&client](const chs::Socket & socket){ return client.getDescriptor() == socket.getDescriptor(); });
    if (pos != clientsSockets.end()) {
        spdlog::info("Closing connection with {}", client.getPort());

        client.close();
        clientsSockets.erase(pos);

        auto pollPosition = pollSockets.begin() + (pos - clientsSockets.begin()) + 1;
        pollSockets.erase(pollPosition);
    }
}

void ConnectionHandler::setForWrite(const chs::Socket & client) {
    getPollFd(client).events |= POLLOUT;
}

void ConnectionHandler::unsetForWrite(const chs::Socket & client) {
    getPollFd(client).events &= ~POLLOUT;
}

pollfd &ConnectionHandler::getPollFd(const chs::Socket & client) {
    auto pos = std::find_if(pollSockets.begin(), pollSockets.end(),
                            [&client](const pollfd & fd){ return client.getDescriptor() == fd.fd; });
    return *pos;
}

std::unique_ptr<ConnectionHandler> ConnectionHandler::instance;

ConnectionHandler &ConnectionHandler::getInstance(const std::string& port) {
    if (instance == nullptr) {
        instance = std::make_unique<ConnectionHandler>(port);
    }
    return *instance;
}



