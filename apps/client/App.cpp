//
// Created by hubert on 24.01.2021.
//

#include "App.hpp"

void App::loginAndStart() {
    connectDialog = std::make_unique<ConnectDialog>();
    connect(connectDialog.get(), &ConnectDialog::accepted, this, &App::openCharadesWindow);

    connectDialog->show();
    connectDialog->setWindowState(Qt::WindowState::WindowActive);
}

void App::openCharadesWindow() {
    auto ptr = connectDialog->getCommunicationHandler();
    charadesWindow = std::make_unique<CharadesWindow>(std::move(ptr));

    charadesWindow->show();
    connectDialog->setWindowState(Qt::WindowState::WindowActive);
}
