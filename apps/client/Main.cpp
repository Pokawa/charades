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
#include <QApplication>
#include "ConnectDialog.h"
#include "ServerHandler.hpp"


int main(int argc, char** argv){
    if (argc != 3)
        error(1, 0, "Need 2 args");

    qRegisterMetaType<chs::Message>("chs::Message");

    QApplication a(argc, argv);
    ConnectDialog window;
    window.show();

    auto connected = ServerHandler::getInstance().connectToHost(argv[1], std::stoi(argv[2]));
    if (connected) {
        auto fd = ServerHandler::getInstance().getSocketFD();
        auto loginMessage = chs::constructMessage(chs::MessageType::LOG_IN_REQUEST, "hubcio");
        send(fd, loginMessage.data(), loginMessage.size(), 0);
    }

    return a.exec();
}

