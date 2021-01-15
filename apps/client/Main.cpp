//
// Created by hubert on 29.12.2020.
//

#include <error.h>
#include <netdb.h>
#include <cerrno>
#include <OutgoingMessageQueue.hpp>
#include <Message.hpp>
#include <spdlog/spdlog.h>
#include <IncomingMessageQueue.hpp>
#include <unistd.h>
#include "spdlog/fmt/bin_to_hex.h"


int main(int argc, char** argv){
    if (argc != 3)
        error(1, 0, "Need 2 args");

    // Resolve arguments to IPv4 address with a port number
    addrinfo *resolved, hints = {.ai_flags = 0, .ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
    int res = getaddrinfo(argv[1], argv[2], &hints, &resolved);
    if (res || !resolved)
        error(1, 0, "getaddrinfo: %s", gai_strerror(res));

    // create socket
    int sock = socket(resolved->ai_family, resolved->ai_socktype, 0);
    if (sock == -1)
        error(1, errno, "socket failed");

    // attept to connect
    res = connect(sock, resolved->ai_addr, resolved->ai_addrlen);
    if (res)
        error(1, errno, "connect failed");

    chs::Socket webSocket{sock, {}};
    // free memory
    freeaddrinfo(resolved);


    chs::OutgoingMessageQueue queue{webSocket};
    chs::IncomingMessageQueue inQueue{webSocket};

    auto login = chs::constructMessage(chs::MessageType::LOG_IN, "hubcio");
    queue.putMessage(login);
    queue.sendMessages();

    sleep(2);

    inQueue.readMessages();
    auto mess = inQueue.getMessage();
    spdlog::info("message: {}", mess);

    auto logout = chs::constructMessage(chs::MessageType::LOG_OUT);
    queue.putMessage(logout);
    queue.sendMessages();

    return 0;
}

