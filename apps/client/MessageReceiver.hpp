//
// Created by hubert on 22.01.2021.
//

#ifndef CHARADES_MESSAGERECEIVER_HPP
#define CHARADES_MESSAGERECEIVER_HPP


#include <QThread>
#include <Message.hpp>

class MessageReceiver : public QThread{
    Q_OBJECT
protected:
    void run() override;
    int socketFD;

public:
    explicit MessageReceiver(int socketFD);

signals:
    void messageReceived(const chs::Message& message);
    void errorOccurred();
    void serverHangUp();
};


#endif //CHARADES_MESSAGERECEIVER_HPP
