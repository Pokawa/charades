//
// Created by hubert on 22.01.2021.
//

#ifndef CHARADES_COMMUNICATIONHANDLER_HPP
#define CHARADES_COMMUNICATIONHANDLER_HPP


#include <string>
#include <QObject>
#include <Message.hpp>
#include <OutgoingMessageQueue.hpp>
#include "MessageReceiver.hpp"
#include "cpptime.h"

class CommunicationHandler : public QObject {
    Q_OBJECT

    int socketFD;
    MessageReceiver asyncMessageReceiver;
    chs::MessageType lastRequestToConfirm;
    chs::OutgoingMessageQueue outgoingMessageQueue;

    static CppTime::Timer timer;

public:
    explicit CommunicationHandler(int socketFD);
    ~CommunicationHandler() override;
    [[nodiscard]] int getSocketFD() const;
    void disconnectFromHost();
    void stopMessageReceiver();
    static int connectToHost(const std::string& host, int port);

private:
    void sendMessage(const chs::Message& message);
    void sendRequest(chs::MessageType request);

public slots:
    void handleMessage(chs::Message message);
    void drawLineRequest(QPoint pt1, QPoint pt2, const QColor& color);
    void logInRequest(const std::string& username);
    void roomsInfoRequest();
    void joinRoomRequest(int roomNumber);
    void exitRoomRequest();
    void newRoomRequest();
    void startGameRequest();
    void stopGameRequest();
    void enterDrawingQueueRequest();
    void leaveDrawingQueueRequest();
    void sendChatMessageRequest(const std::string& message);
    void clearRequest();
    void logOut();

signals:
    void loginSuccessful();
    void loginFailed();
    void roomsInfoRespond(chs::Message message);
    void joinedRoom();
    void inGameInfoRespond(chs::Message message);
    void joiningRoomFailed();
    void linePainted(QPoint pt1, QPoint pt2, QColor color);
    void drawingCleared();
    void receivedChatMessage(const std::string& message);
    void receivedServerMessage(const std::string& message);
    void receivedCharadesWord(const std::string& word);
    void connectionLost();
};


#endif //CHARADES_COMMUNICATIONHANDLER_HPP
