//
// Created by hubert on 22.01.2021.
//

#ifndef CHARADES_COMMUNICATIONHANDLER_HPP
#define CHARADES_COMMUNICATIONHANDLER_HPP


#include <string>
#include <QObject>
#include <Message.hpp>
#include "MessageReceiver.hpp"

class CommunicationHandler : public QObject {
    Q_OBJECT

    int socketFD;
    MessageReceiver asyncMessageReceiver;
    chs::MessageType lastRequest;

public:
    explicit CommunicationHandler(int socketFD);
    ~CommunicationHandler();
    [[nodiscard]] int getSocketFD() const;
    void disconnectFromHost();
    void stopMessageReceiver();
    static int connectToHost(const std::string& host, int port);
    void sendMessage(const chs::Message& message);

    void logInRequest(const std::string& username);
    void roomsInfoRequest();

public slots:
    void handleMessage(chs::Message message);

signals:
    void loginSuccessful();
    void loginFailed();
    void roomsInfoRespond(int roomNumber, const std::vector<std::string>& names);

};


#endif //CHARADES_COMMUNICATIONHANDLER_HPP
