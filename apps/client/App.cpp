//
// Created by hubert on 24.01.2021.
//

#include "App.hpp"

void App::loginAndStart() {
    if (charadesWindow != nullptr) {
        charadesWindow->close();
    }

    connectDialog = std::make_unique<ConnectDialog>();
    connect(connectDialog.get(), &ConnectDialog::accepted, this, &App::openCharadesWindow);

    connectDialog->show();
    connectDialog->setWindowState(Qt::WindowState::WindowActive);
}

void App::openCharadesWindow() {
    auto ptr = connectDialog->getCommunicationHandler();
    connect(ptr.get(), &CommunicationHandler::connectionLost, this, &App::loginAndStart);
    connect(ptr.get(), &CommunicationHandler::connectionLost, this, &App::setConnectionLostLabel);

    auto username = connectDialog->getUserName();
    charadesWindow = std::make_unique<CharadesWindow>(std::move(ptr), username);

    charadesWindow->show();
    connectDialog->setWindowState(Qt::WindowState::WindowActive);
}

void App::setConnectionLostLabel() {
    connectDialog->setConnectionLostLabel();
}
