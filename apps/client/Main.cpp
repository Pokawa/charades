//
// Created by hubert on 29.12.2020.
//

#include <error.h>
#include <netdb.h>
#include <cerrno>
#include <OutgoingMessageQueue.hpp>
#include <Message.hpp>
#include <spdlog/spdlog.h>
#include <iostream>
#include <IncomingMessageQueue.hpp>
#include <unistd.h>
#include <string>


void waitForEnter(){
    do
    {
        std::cout << '\n' << "Press a key to continue...";
    } while (std::cin.get() != '\n');
}

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

    auto login = chs::constructMessage(chs::MessageType::LOG_IN_REQUEST, "uzyz");
    queue.putMessage(login);
    queue.sendMessages();

    waitForEnter();

    inQueue.readMessages();
    auto mess = inQueue.getMessage();
    if (chs::getMessageType(mess) == chs::MessageType::OK_RESPOND) {
        spdlog::info("udane połączenie");

        waitForEnter();
        auto newRoom = chs::constructMessage(chs::MessageType::NEW_ROOM_REQUEST);
        queue.putMessage(newRoom);
        queue.sendMessages();

        waitForEnter();
        auto roomsInfo = chs::constructMessage(chs::MessageType::ROOMS_INFO_REQUEST);
        queue.putMessage(roomsInfo);
        queue.sendMessages();

        waitForEnter();
        inQueue.readMessages();
        auto info = inQueue.getMessage();
        if (chs::getMessageType(info) == chs::MessageType::ROOM_INFO_RESPOND) {
            auto [roomNumber, players] = chs::deconstructMessage<int, std::string>(info);
            spdlog::info("room info: {} {}", roomNumber, players);
        }

        info = inQueue.getMessage();
        if (chs::getMessageType(info) == chs::MessageType::ROOM_INFO_RESPOND) {
            auto [roomNumber, players] = chs::deconstructMessage<int, std::string>(info);
            spdlog::info("room info: {} {}", roomNumber, players);
        }


        waitForEnter();
        auto logout = chs::constructMessage(chs::MessageType::LOG_OUT_REQUEST);
        queue.putMessage(logout);
        queue.sendMessages();
    }
    waitForEnter();
    return 0;
}

