//
// Created by hubert on 29.12.2020.
//

#include "IOHandler.hpp"

void IOHandler::addClient(const chs::WebSocket &socket) {
    incomingQueues.emplace(socket.getDescriptor(), socket);
    outgoingQueues.emplace(socket.getDescriptor(), socket);
}

void IOHandler::removeClient(const chs::WebSocket &socket) {
    incomingQueues.erase(socket.getDescriptor());
    outgoingQueues.erase(socket.getDescriptor());
}

void IOHandler::receiveFrom(const chs::WebSocket &socket) {
    incomingQueues.at(socket.getDescriptor()).readMessages();
}

void IOHandler::sendTo(const chs::WebSocket &socket) {
    auto& queue = outgoingQueues.at(socket.getDescriptor());
    queue.sendMessages();

    if (queue.isBlocked()) {
        connectionHandler.setForWrite(socket);
    } else {
        connectionHandler.unsetForWrite(socket);
    }
}

void IOHandler::putMessage(const chs::WebSocket &socket, const chs::Message & message) {
    outgoingQueues.at(socket.getDescriptor()).putMessage(message);
    connectionHandler.setForWrite(socket);
}

chs::Message IOHandler::getMessage(const chs::WebSocket &socket) {
    return incomingQueues.at(socket.getDescriptor()).getMessage();
}

bool IOHandler::isMessageToGet(const chs::WebSocket &socket) {
    return incomingQueues.at(socket.getDescriptor()).isNotEmpty();
}

IOHandler::IOHandler(ConnectionHandler& connectionHandler) : connectionHandler(connectionHandler) {
}


