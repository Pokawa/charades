//
// Created by hubert on 22.01.2021.
//

#ifndef CHARADES_SERVERHANDLER_HPP
#define CHARADES_SERVERHANDLER_HPP


#include <string>
#include <QObject>
#include <Message.hpp>

class ServerHandler : public QObject {
    Q_OBJECT

    int socketFD;
    static ServerHandler instance;

public:
    void operator=(const ServerHandler &) = delete;
    ServerHandler(ServerHandler &other) = delete;
    static ServerHandler& getInstance();

    bool connectToHost(const std::string& host, int port);
    int getSocketFD();

public slots:
    void handleMessage(const chs::Message& message);

private:
    ServerHandler();
};


#endif //CHARADES_SERVERHANDLER_HPP
