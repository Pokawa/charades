//
// Created by hubert on 27.12.2020.
//

#include <arpa/inet.h>
#include <unistd.h>
#include "Socket.hpp"
#include <spdlog/spdlog.h>

chs::Socket::Socket(int fd, sockaddr_in sockInfo)  : descriptor(fd), sockInfo(sockInfo) {}

std::string chs::Socket::getAddress() const {
    return inet_ntoa(sockInfo.sin_addr);
}

uint16_t chs::Socket::getPort() const {
    return ntohs(sockInfo.sin_port);
}

int chs::Socket::getDescriptor() const {
    return descriptor;
}

void chs::Socket::close() {
    ::close(descriptor);
}



