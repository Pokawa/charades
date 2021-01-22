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

    static MessageReceiver instance;

public:
    void operator=(const MessageReceiver &) = delete;
    MessageReceiver(MessageReceiver &other) = delete;
    static MessageReceiver& getInstance();

    MessageReceiver() = default;

signals:
    void messageReceived(const chs::Message& message);
};


#endif //CHARADES_MESSAGERECEIVER_HPP
