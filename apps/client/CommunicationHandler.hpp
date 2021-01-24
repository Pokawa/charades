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
    ~CommunicationHandler() override;
    [[nodiscard]] int getSocketFD() const;
    void disconnectFromHost();
    void stopMessageReceiver();
    static int connectToHost(const std::string& host, int port);
    void sendMessage(const chs::Message& message);

    //REQUESTS
    void logInRequest(const std::string& username);
    void roomsInfoRequest();
    void joinRoomRequest(int roomNumber);
    void newRoomRequest();


public slots:
    void handleMessage(chs::Message message);

signals:
    void loginSuccessful();
    void loginFailed();
    void roomsInfoRespond(int roomNumber, std::string joinedNames);
    void joinedRoom();
    void joiningRoomFailed();

};


#endif //CHARADES_COMMUNICATIONHANDLER_HPP
