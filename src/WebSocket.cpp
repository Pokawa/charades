//
// Created by hubert on 27.12.2020.
//

#include <arpa/inet.h>
#include <unistd.h>
#include "WebSocket.hpp"

chs::WebSocket::WebSocket(int fd, sockaddr_in sockInfo)  : descriptor(fd), sockInfo(sockInfo) {}

std::string chs::WebSocket::getAddress() const {
    return inet_ntoa(sockInfo.sin_addr);
}

uint16_t chs::WebSocket::getPort() const {
    return ntohs(sockInfo.sin_port);
}

int chs::WebSocket::getDescriptor() const {
    return descriptor;
}

void chs::WebSocket::close() const {
    ::close(descriptor);
}

chs::WebSocket::~WebSocket() {
    close();
}


