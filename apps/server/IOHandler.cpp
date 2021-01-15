//
// Created by hubert on 29.12.2020.
//

#include "IOHandler.hpp"

void IOHandler::addClient(const chs::Socket &socket) {
    incomingQueues.emplace(socket.getDescriptor(), socket);
    outgoingQueues.emplace(socket.getDescriptor(), socket);
}

void IOHandler::removeClient(const chs::Socket &socket) {
    incomingQueues.erase(socket.getDescriptor());
    outgoingQueues.erase(socket.getDescriptor());
}

void IOHandler::receiveFrom(const chs::Socket &socket) {
    incomingQueues.at(socket.getDescriptor()).readMessages();
}

void IOHandler::sendTo(const chs::Socket &socket) {
    auto& queue = outgoingQueues.at(socket.getDescriptor());
    queue.sendMessages();

    if (queue.isBlocked()) {
        connectionHandler.setForWrite(socket);
    } else {
        connectionHandler.unsetForWrite(socket);
    }
}

void IOHandler::putMessage(const chs::Socket &socket, const chs::Message & message) {
    outgoingQueues.at(socket.getDescriptor()).putMessage(message);
    connectionHandler.setForWrite(socket);
}

chs::Message IOHandler::getMessage(const chs::Socket &socket) {
    return incomingQueues.at(socket.getDescriptor()).getMessage();
}

bool IOHandler::isMessageToGet(const chs::Socket &socket) {
    return incomingQueues.at(socket.getDescriptor()).isNotEmpty();
}

IOHandler::IOHandler(ConnectionHandler& connectionHandler) : connectionHandler(connectionHandler) {
}

void IOHandler::putMessages(const chs::Socket &socket, const std::vector<chs::Message> & messages) {
    auto& queue = outgoingQueues.at(socket.getDescriptor());
    auto putMessage = [&queue](chs::Message & message){queue.putMessage(std::move(message)); };
    std::for_each(messages.begin(), messages.end(), putMessage);
    connectionHandler.setForWrite(socket);
}


