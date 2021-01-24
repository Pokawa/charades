#include "CharadesWindow.hpp"
#include "ui_CharadesWindow.h"
#include <fmt/format.h>

CharadesWindow::CharadesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CharadesWindow)
{
    ui->setupUi(this);
}

CharadesWindow::~CharadesWindow()
{
    delete ui;
}

void CharadesWindow::on_joinButton_clicked()
{

}

void CharadesWindow::on_newRoomButton_clicked()
{

}

void CharadesWindow::on_refreshButton_clicked()
{
    ui->roomsList->clear();
    communicationHandler->roomsInfoRequest();
}

CharadesWindow::CharadesWindow(std::unique_ptr<CommunicationHandler> ptr) : CharadesWindow() {
    communicationHandler = std::move(ptr);
    connect(communicationHandler.get(), &CommunicationHandler::roomsInfoRespond, this, &CharadesWindow::roomsInfoRespond);


    emit ui->refreshButton->clicked();
}

void CharadesWindow::roomsInfoRespond(int roomNumber, std::string joinedNames) {
    std::replace(joinedNames.begin(), joinedNames.end(), ';', ' ');
    auto itemString = fmt::format("Room {} Players: {}", roomNumber, joinedNames);
    ui->roomsList->addItem(QString::fromStdString(itemString));
}


